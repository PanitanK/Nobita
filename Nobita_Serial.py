import serial
import time



def run() :
    try :
        arduino = serial.Serial(port='COM5', baudrate=9600, timeout=.5)
        enable = input("Send Enable : ")
        enable = enable.lower() 
        if enable == "enable" :
            arduino.write(1)
            try :
                while True :
                    start = time.time()
                    data = arduino.readline()
                    if data != ( b'0\r\n' or b'' )  :
                        data = str(data)
                        buffer = ""
                        j = 2 
                        while data[j] != '\\' :
                            buffer = buffer + data[j]
                            j = j+1
                        #print(buffer)
                        buf = ""
                        param = []
                        for i in buffer :
                            if i == "," :
                                param.append(float(buf))
                                buf = ""
                            else :
                                buf = buf + i 
                        param.append(float(buf))
                        print(param)
                        end = time.time()
                        print(end - start)
                            ######################3

                            #6 Variables x_acceleration_in(g) , y_acceleration_in(g) , z_acceleration_in(g)
                            # x_omega,y_omega and z_omega is in an array named "param " You can put you code here to manage Serial data

                            #######################

                    else :
                        arduino.write(1)
            except KeyboardInterrupt :
                time.sleep(1)
                print("\n")
                while (arduino.in_waiting) : 
                    arduino.readline()
                    break
                arduino.close()
                
                run()
    except KeyboardInterrupt or EOFError :
        time.sleep(1)
        print("\n")
        print("Terminated")
        arduino.close()


run()
