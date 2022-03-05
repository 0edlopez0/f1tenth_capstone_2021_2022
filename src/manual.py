import rospy
from ucsb_f1tenth_capstone_host.msg import effortMsg
import pygame
from pygame.locals import *

pygame.init()
screen = pygame.display.set_mode((240,240))
pygame.display.set_caption('Pi Car')

w = pygame.key.get_pressed()[pygame.K_w]

def manual():
    steer = 14.5
    duty_cycle = 0
    pub = rospy.Publisher('control_effort', effortMsg, queue_size=10)
    rospy.init_node('manual', anonymous=True)
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        msg = effortMsg()
        
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT:
                    steer = 11
                elif event.key == pygame.K_RIGHT:
                    steer = 18
                elif event.key == pygame.K_UP:
                    duty_cycle = 0.1
                elif event.key == pygame.K_DOWN:
                    duty_cycle = -0.1
                msg.steer = steer
        msg.duty_cycle = duty_cycle
        pub.publish(msg)
        rate.sleep()


if __name__ == '__main__':
    try:
        manual()
    except rospy.ROSInterruptException:
        pass
