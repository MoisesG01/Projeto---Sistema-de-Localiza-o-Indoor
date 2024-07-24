import turtle
import paho.mqtt.client as mqtt

esp1_value = 0
esp2_value = 0
esp3_value = 0

turtle.title("Indoor Positioning System")

# Definir as dimensões da sala (em metros)
largura_sala = 21
altura_sala = 18.6

# Definir as coordenadas dos objetos na sala (em metros)
x_ESP1 = 9
y_ESP1 = 4.60

x_ESP2 = 13.80
y_ESP2 = 4.60

x_ESP3 = 13.80
y_ESP3 = 13.20

# Definir a escala
escala = 0.02

# Definir as margens (em pixels)
margem_esquerda = 2
margem_superior = 2

# Inicializar a tela
tela = turtle.Screen()

tela.bgpic("planta/Planta_Laboratório.png")

# Definir a escala da tela
tela.setworldcoordinates(0, 0, largura_sala + margem_esquerda, altura_sala + margem_superior)

# Criar uma tartaruga para desenhar a sala
tartaruga = turtle.Turtle()

# Desenhar a sala
tartaruga.penup()
tartaruga.goto(0, 0)
tartaruga.pendown()
tartaruga.goto(largura_sala, 0)
tartaruga.goto(largura_sala, altura_sala)
tartaruga.goto(0, altura_sala)
tartaruga.goto(0, 0)

# Desenhar os objetos
ESP1 = turtle.Turtle()
ESP1.penup()
ESP1.goto(x_ESP1, y_ESP1)
ESP1.shape("square")
ESP1.write("ESP1", align="center", font=("Arial", 16, "bold"))
print(f"Objeto 1: ({x_ESP1 :.2f}, {y_ESP1 :.2f})")

p = x_ESP2 

ESP2 = turtle.Turtle()
ESP2.penup()
ESP2.goto(p, y_ESP2)
ESP2.shape("square")
ESP2.write("ESP2", align="center", font=("Arial", 16, "bold"))
print(f"Objeto 2: ({x_ESP2 :.2f}, {y_ESP2 :.2f})")

q = x_ESP3 
r = y_ESP3 

ESP3 = turtle.Turtle()
ESP3.penup()
ESP3.goto(q, r)
ESP3.shape("square")
ESP3.write("ESP3", align="center", font=("Arial", 16, "bold"))
print(f"Objeto 3: ({x_ESP3 / escala:.2f}, {y_ESP3 / escala:.2f})")

Beacon = turtle.Turtle()
Beacon.penup()
Beacon.speed(1)
Beacon.shape("turtle")
Beacon.color("red")
Beacon.pencolor("black")

def update_beacon_position():
    global esp1_value, esp2_value, esp3_value
    
    e = esp1_value
    f = esp2_value
    g = esp3_value 

    x = ((e )**2 - (f )**2 + p**2) / (2*p)

    y = (((e + 5 )**2 - (g )**2 + q**2 + r**2)/(2*r)) - (q/r) * (x )

    # Calculate the angle between the current position and the new position
    angle = Beacon.towards(x, y)

    # Rotate the turtle to face the new position
    Beacon.setheading(angle)

    # Move the turtle to the new position
    # Beacon.goto(x, y + 5)
    
    turtle.Screen().update() # Update the screen
    print(f"Beacon position: ({x:.2f}, {y:.2f})")
    Beacon.goto(x, y)

def on_connect(client, userdata, flags, rc):
    # This will be called once the client connects
    print(f"Connected with result code {rc}")
    # Subscribe here!
    client.subscribe("MOISES/ESP1")
    client.subscribe("MOISES/ESP2")
    client.subscribe("MOISES/ESP3")
    
def on_message(client, userdata, msg):
    global esp1_value, esp2_value, esp3_value
    
    if msg.topic == "MOISES/ESP1":
        esp1_value = float(msg.payload)
    elif msg.topic == "MOISES/ESP2":
        esp2_value = float(msg.payload)
    elif msg.topic == "MOISES/ESP3":
        esp3_value = float(msg.payload)
    
    print(f"ESP1 {esp1_value}", "metros")
    print(f"ESP2 {esp2_value}", "metros")
    print(f"ESP3 {esp3_value}", "metros")
    
    update_beacon_position() # Call the function to update the Beacon position
        
client = mqtt.Client("mqtt-test") # client ID "mqtt-test"
client.on_connect = on_connect
client.on_message = on_message
client.connect('test.mosquitto.org', 1883)
client.loop_forever()  # Start networking

# Esperar até que o usuário feche a janela
turtle.done()
