import rospy
import pyvesc
import serial

port_object = serial.Serial("/dev/ttyACM0", 11520, timeout=0.1)


def write_to_port(message):
    packet = pyvesc.encode(message)
    port_Object.write(packet)


def set_duty_cycle(value):
    message = pyvesc.SetDutyCycle(value)
    write_to_port(message)


def set_current(value):
    message = pyvesc.SetCurrent(value)
    write_to_port(message)



def control_effort_heard(eff):
    #TO DO: give vesc power
    set_duty_cycle(int(1e5))
    set_current(eff)
    port_Object.flush()



def actuators():
    rospy.init_node('actuators', anonymous = True)

    rospy.Subscriber('control_effort', int, control_efort_heard)

    rospy.spin()



if __name__ ==  '__main__':
    actuators()
