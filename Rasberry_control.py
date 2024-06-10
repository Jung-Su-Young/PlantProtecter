import serial
import time

# 두 개의 Arduino 보드를 위한 시리얼 포트 설정
arduino1 = serial.Serial('/dev/ttyACM0', 9600)  # 온습도 측정을 위한 Arduino
arduino2 = serial.Serial('/dev/ttyACM1', 9600)  # 먼지 센서를 위한 Arduino
arduino3 = serial.Serial('/dev/ttyACM2', 9600)  # 부저  
yellow = 1  # 감지되었을 때 돌아가는지 확인하기 위한  
time.sleep(2)  # 시리얼 포트가 열릴 시간을 확보하기 위한 대기 시간

try:
    while True:  
        # 8시간마다 'RUN' 명령어(0x01)를 전송
        arduino1.write(b'\x01')  # 첫 번째 Arduino에 'RUN' 명령 전송
        arduino2.write(b'\x01')  # 두 번째 Arduino에 'RUN' 명령 전송
        if yellow == 1:
            arduino3.write(b'\x01')  # 세 번째 Arduino에 'RUN' 명령 전송
        
        print("Sent 'RUN' command to all Arduinos")
        
        # 2분간 실행  
        time.sleep(2 * 60)  # 2분간 실행  
        
        # 'STOP' 명령어(0x00)를 전송
        arduino1.write(b'\x00')  # 첫 번째 Arduino에 'STOP' 명령 전송
        arduino2.write(b'\x00')  # 두 번째 Arduino에 'STOP' 명령 전송
        arduino3.write(b'\x00')  # 세 번째 Arduino에 'STOP' 명령 전송
        print("Sent 'STOP' command to all Arduinos")
        
        # 8시간 동안 대기
        time.sleep(8 * 60 * 60)  # 8시간 후 재검사 실행  

except KeyboardInterrupt:
    # 프로그램이 종료될 때 Arduino 연결 해제
    arduino1.close()  # 첫 번째 Arduino 연결 해제
    arduino2.close()  # 두 번째 Arduino 연결 해제
    arduino3.close()  # 세 번째 Arduino 연결 해제
    print("Program terminated")


