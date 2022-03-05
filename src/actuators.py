import rospy
import RPi.GPIO as IO
import time
from newPyvesc import VESC
from ucsb_f1tenth_capstone_host.msg import effortMsg

#port_object = serial.Serial("/dev/ttyACM0", 11520, timeout=0.1)

output_pin = 26

IO.setwarnings(False)
IO.setmode(IO.BCM)
IO.setup(output_pin,IO.OUT)

servo_freq = 100
servo = IO.PWM(output_pin, servo_freq)

serial_port = '/dev/ttyACM0'

def set_motor(duty_cycle):
    with VESC(serial_port=serial_port) as motor:
        motor.set_duty_cycle(duty_cycle)

def set_steer(steer):
    servo.ChangeDutyCycle(steer)

def control_effort_heard(effort):
    #TO DO: give vesc power
    set_motor(effort.duty_cycle)
    set_steer(effort.steer)
    rospy.loginfo(effort.steer)



def actuators():
    rospy.init_node('actuators', anonymous = True)
    servo.start(14.5)

    rospy.Subscriber('control_effort', effortMsg, control_effort_heard)

    rospy.spin()




if __name__ ==  '__main__':
    actuators()
