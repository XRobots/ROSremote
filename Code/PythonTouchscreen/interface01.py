#!/usr/bin/env python
import pygame 
import sys 
import rospy
from std_msgs.msg import String

pub = rospy.Publisher('touchscreen', String, queue_size=10)
rospy.init_node('screen', anonymous=True)
rate = rospy.Rate(10) # 10hz  
  
# initializing the constructor 
pygame.init() 
  
# screen resolution 
res = (800,480) 
  
# opens up a window 
screen = pygame.display.set_mode(res) 
  
# white color 
color = (255,255,255) 
  
# light shade of the button 
color_light = (170,170,170) 
  
# dark shade of the button 
color_dark = (100,100,100) 
  
# stores the width of the 
# screen into a variable 
width = screen.get_width() 
  
# stores the height of the 
# screen into a variable 
height = screen.get_height() 
  
# defining a font 
smallfont = pygame.font.SysFont('Corbel',35) 
  
# rendering a text written in 
# this font 
text1 = smallfont.render('quit' , True , color)
text2 = smallfont.render('hello' , True , color) 
text3 = smallfont.render('goodbye' , True , color) 

# fills the screen with a color 
screen.fill((0,0,0)) 

#draw buttons
pygame.draw.rect(screen,color_dark,[400,300,140,50])
screen.blit(text1, (405,305))

pygame.draw.rect(screen,color_dark,[100,200,140,50])
screen.blit(text2, (105,205))

pygame.draw.rect(screen,color_dark,[100,300,140,50])
screen.blit(text3, (105,305))

hello_str = "hello"
goodbye_str = "goodbye"

# updates the frames of the game 
pygame.display.update()

def hello():
    print("hello")
    pub.publish(hello_str)

def goodbye():
    print("goodbye")
    pub.publish(goodbye_str)

def quit():
    pygame.quit()
  
while True: 
     
    for ev in pygame.event.get(): 
              
        #checks if a mouse is clicked 
        if ev.type == pygame.FINGERUP or ev.type == pygame.MOUSEBUTTONDOWN: 
              
            #other functions   
            if 100 <= mouse[0] <= 240 and 200 <= mouse[1] <= 250:  
                hello()  

            #other functions   
            if 100 <= mouse[0] <= 240 and 300 <= mouse[1] <= 350:  
                goodbye()      

            if 400 <= mouse[0] <= 540 and 300 <= mouse[1] <= 350:  
                quit()   
    
      
    # stores the (x,y) coordinates into 
    # the variable as a tuple 
    mouse = pygame.mouse.get_pos() 

    #print(mouse[0])
    #print(mouse[1])              
      
     
