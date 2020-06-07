import socket 
import select 
import signal
import sys 
import pickle
import time
import threading 
import random
import os
import fileinput

killSwitch=0 #signifies if node is exiting the network


allThreads=[]
list_of_peers=[]

oldTags=[]
myTag=""

welcomeText=0

reconnect=0 # SET IF PARENT LEAVES
newParent=0 

parentThread=[]
closeParenThread=0
peerThreads=[]
closePeerThreads=0
reconnectThreads=[]
closeReconnectThread=0

#==============================================INTIALIZATION================================================================#

# CREATE SOCKETS 

## SERVER SOCKET FOR ACCEPTING NEW NODES INTO THE NETWORK
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) 

## CLIENT SOCKET TO JOIN INTO THE EXISTING NETWORK
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#ARG HELP
if len(sys.argv) != 3: 
	print("Correct usage: script, your ID, your port number")
	exit() 

# ARG PARSING 
IP_address = "127.0.0.1"

noParent=0	# SIGNIFIES IF NODE IS FIRST IN THE NETWORK
myID=sys.argv[1]  
myPort = int(sys.argv[2]) 

availablePorts=[]
file=open("P2P/ports.txt")
for line in (list(file)):
		availablePorts.append(line.rstrip())	
for port in availablePorts:
	if(port!=''):
		break
if(len(availablePorts)==0): port=""
if(port == ""):
	noParent=1
else:
	parentPort=int(port)
file.close()
file=open('P2P/ports.txt','a')
file.writelines(["\n"+str(myPort)])
file.close()
#==============================================CATCH CTRL-C================================================================#
#==========================================DISCONNECT FROM PARENT==========================================================# 
#====================================AND POINT CHILDREN PEERS TO PARENT ===================================================#


def exit_handler(sig, frame):
	global killSwitch
	print('you wanna exit? (Y/N)')
	answer=input()
	print("YOU SAID "+answer)
	if(answer=="Y" or  answer=="y"):
		print("KILLING")
		killSwitch=1
		time.sleep(3)

		a_file = open("P2P/ports.txt", "r")
		lines = a_file.readlines()
		a_file.close()

		new_file = open("P2P/ports.txt", "w")
		for line in lines:
			if (line.strip("\n") != (str(myPort)) ):
				new_file.write(line)
		new_file.close()
		sys.exit(0)
	return

signal.signal(signal.SIGINT, exit_handler)

#==================================== RECONNECTION HANDLER IF PARENT LEAVES ===================================================#


def reconnectHandler():
	global killSwitch
	global newParent
	global reconnect
	global list_of_peers
	global noParent
	global closeParenThread
	global closePeerThreads 	
	global closeReconnectThread


	while not reconnect: 
		if(killSwitch):
			closeReconnectThread=1
			return
	ports=[]
	for line in reversed(list(open("P2P/ports.txt"))):
    		ports.append(line.rstrip())	
	for port in ports:
		if(port!=''):
			break
	newParent=int(port)

	print("< reconnect started >")
	print("< new Parent port "+str(newParent)+" >")
	client= socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	client.connect((IP_address,newParent))
	x=threading.Thread(target=parentCommuncation,args=(client,),daemon=True)
	parentThread.append(x)
	x.start()

	print("<DONE RECONNECTING>")
	reconnect=0
	reconnectHandler()

x=threading.Thread(target=reconnectHandler,daemon=True)
reconnectThreads.append(x)
x.start()

#====================== CONNECT TO PARENT - AND COMMUNICATION BETWEEN NODE AND PARENT =====================================#  
# CONNECT TO PARENT


def parentCommuncation(socket):
	global killSwitch
	global list_of_peers
	global reconnect
	global myID
	global myPort
	global welcomeText
	global myTag
	global oldTags
	global newParent
	global reconnect
	global closeParenThread
	global closePeerThreads 	
	global closeReconnectThread

	if not welcomeText:
		myTag=str(random.randint(0,999))
		socket.sendall((myTag+ " <" +myID+"> "+ "Sup everybody! this is"+myID).encode('utf-8'))
		welcomeText=1
	list_of_peers.append(socket)

	breakWhile=0
	while True: 
		# DISCONNECT ON SIGINT
		if (killSwitch):
			print("KILLSWITCH")
			print("<sent byebye to parent>")
			socket.sendall(("seeya").encode('utf-8'))
			socket.close()
			print("exiting")
			closeParenThread=1
			
			break
		# MAINTAINS A LIST OF POSSIBLE INPUT STREAMS 
		sockets_list = [sys.stdin, socket] 
		read_sockets,write_socket, error_socket = select.select(sockets_list,[],[]) 
		for socks in read_sockets: 
			if socks == socket: 
				
				message = (socks.recv(2048)).decode('utf-8') 
				
#				print("<LOG MESSAGE > "+"<"+message+">")
				if(message==''):
					reconnect=1
					print("GOT ERROR")
					list_of_peers.remove(socket)
					print("removed parent from peers too")
					socks.close()
					print("thread exit")
					closeParenThread=1
					breakWhile=1
					break
					
				elif (message=="seeya"):
					print("parent says byebye")
					reconnect=1
					list_of_peers.remove(socket)
					print("removed parent from peers")
					socks.close()
					print("thread exit")
					closeParenThread=1
					breakWhile=1
					break

				elif(message=="ping me your port"):
					socks.sendall(str(myPort).encode('utf-8'))
				else:
					# PRINT RECEIVED MSGstart_new_thread(parentCommuncation,(client,))

					tagAndMessage=message.split(' ', 1)
					if((tagAndMessage[0] not in oldTags) and len(tagAndMessage)>1):
						print(tagAndMessage[1]) 
						forward(message,socks,"None")
						oldTags.append(tagAndMessage[0])

			else: 
				message = input()
				messageWithInfo="<" + myID+"> "+ message
				myTag=str(random.randint(0,999))
				oldTags.append(myTag)
				socket.sendall((myTag +" "+messageWithInfo).encode('utf-8'))
				sys.stdout.write("<You>") 
				sys.stdout.write(message) 
				forward(messageWithInfo,None,myTag)
				sys.stdout.flush() 
		if(breakWhile): break
	return
if not noParent:
	client.connect((IP_address,parentPort))
	x=threading.Thread(target=parentCommuncation,args=(client,),daemon=True)
	parentThread.append(x)
	x.start()



#===================================== ACT AS SERVER FOR INCOMING NODES ===================================================#
# BIND SOCKET
server.bind((IP_address, myPort)) 

#LISTEN TO 100 CONNECTIONS MAX.
server.listen(100) 


def peerthread(conn, addr): 

	global killSwitch
	global noParent 
	global myPort
	global client
	global parentPort
	global allThreads
	global closeParenThread
	global closePeerThreads 	
	global closeReconnectThread

	if noParent:
		conn.sendall(("ping me your port").encode('utf-8'))
#		print("<no parent for me, asked for port of child>")
		print(((conn.recv(2048)).decode('utf-8')))
		parentPort=int((conn.recv(2048)).decode('utf-8'))
#		print("<child says "+str(parentPort))
		client.connect((IP_address,parentPort))
		noParent=0
		x=threading.Thread(target=parentCommuncation,args=(client,),daemon=True)
		parentThread.append(x)
		x.start()

#	conn.sendall(("welcome to P2P chat! "+ "<" + str(addr[0]) + " : " + str(addr[1]) + "> ").encode('utf-8')) 

	while True:
		if killSwitch:
			conn.sendall(("seeya").encode('utf-8'))

			if not noParent:
				conn.sendall(str(parentPort).encode('utf-8'))
			else:
				conn.sendall(("noParent").encode('utf-8'))
			conn.close()
			closePeerThreads=1	
			break


		try: 
			message = (conn.recv(2048)).decode('utf-8')
			if message:
				if message=="seeya":
					break
				# PRINT RECEIVED MSG
				tagAndMessage=message.split(' ', 1)
				if(tagAndMessage[0] not in oldTags):
					print(tagAndMessage[1]) 
					oldTags.append(tagAndMessage[0])					
					# FORWARD TO OTHER PEERS
					message_to_send=message
					# message_to_send = "<" + str(addr[0]) + " : " + str(addr[1]) + "> " + message
					forward(message_to_send, conn,"None") 
				
			else: 
				remove(conn) 

		except: 
			continue

# FORWARD TO CONNECTED CLIENTS
def forward(message, connection,tag): 
	for peers in list_of_peers: 
		if peers!=connection: 
			try: 
				if(tag=="None"):
					peers.sendall((message).encode('utf-8')) 
				else:
					peers.sendall((tag+" "+ message).encode('utf-8')) 
			except: 
				peers.close() 

				# IF THE LINK IS BROKEN, WE REMOVE THE PEER 
				remove(peers) 

# REMOVE THE CONNECTION
def remove(connection): 
	if connection in list_of_peers: 
		list_of_peers.remove(connection) 

while True: 
	if(killSwitch): break
	# ACCEPT INCOMING CONNECTIONS
	conn, addr = server.accept() 
	
	# APPEND THE NEW CONNECTION
	list_of_peers.append(conn) 

	# PRINTS THE ADDRESS AND PORT OF THE NEW CONNCTION THAT JUST CONNECTED 
#	print(str(addr[0]) + " : " + str(addr[1]) + " connected")

	# CREATES AN INDIVIDUAL THREAD FOR EVERY PEER CONNECTION
	x=threading.Thread(target=peerthread,args=(conn,addr,),daemon=True)
	peerThreads.append(x)
	x.start()
