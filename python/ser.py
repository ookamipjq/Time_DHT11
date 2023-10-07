import serial, time, sys

def read_serial(serial):
    while True:
        data = serial.readall()
        if data == ' ':
            continue
        else:
            break
        time.sleep(0.1)
    return data

def send_data():
    senddata = time.strftime('%H:%M:%S',time.localtime())
    if(serial.isOpen()):
        serial.write(senddata.encode('utf-8'))
        print(senddata.encode())
    else:
        print('Serial port is not open')

if __name__ == '__main__':
    serial = serial.Serial('COM3', 115200, timeout=1)
    if(serial.isOpen()):
        print('Serial port is open')
    else:
        print('Serial port is not open')

    while True:
        send_data()
        time.sleep(1)
        data = read_serial(serial)
        if data != ' ':
            print("rec:",data)
            break