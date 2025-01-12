import serial
import time
from PIL import Image
import numpy as np

def image_to_grayscale(image_path, output_csv):
    try:
        # 開啟圖片並轉換為灰階
        img = Image.open(image_path).convert('L')  # 確保圖片轉為灰階
        img_array = np.array(img, dtype=int)  # 確保數據為整數

        # 儲存灰階值為 CSV
        np.savetxt(output_csv, img_array, fmt="%d", delimiter=",")
        print(f"灰階值已儲存到 {output_csv}")

        return img_array
    except Exception as e:
        print(f"處理圖片時發生錯誤：{e}")
        exit(1)

def send_grayscale_via_uart(grayscale_values, port, baudrate):
    try:
        # 初始化 UART 串口
        ser = serial.Serial(port, baudrate, timeout=1)
        time.sleep(2)  # 等待串口穩定
        counter = -1
        

        # 逐個傳送灰階數據
        for row in grayscale_values:
            for value in row:
                # 將 NumPy 整數值轉換為 Python 原生整數
              
                int_value = int(value)

                # 確保灰階值在 0-255 範圍
                if not (0 <= int_value <= 255):
                    raise ValueError(f"灰階值超出範圍: {int_value}")

                # 傳送單個整數值，轉為 1 字節
                ser.write(int_value.to_bytes(1, byteorder='big'))
                time.sleep(0.01)  # 根據硬體需求調整間隔時間

                # 計數器更新並輸出
                counter += 1
                binary_value = format(int_value, '08b')  # 8 位二進制格式
               
                print(f"addres: {counter:08X} | integer: {int_value} | binary: {binary_value}")
        print("灰階數據傳輸完成！")
    except serial.SerialException as e:
        print(f"串口錯誤：{e}")
    except Exception as e:
        print(f"發生錯誤：{e}")
    finally:
        ser.close()

# 範例使用
image_path = "person_1_resized_gray.jpg"  # 原始圖片路徑
output_csv = "output_grayscale.csv"  # 灰階值輸出路徑
grayscale_values = image_to_grayscale(image_path, output_csv)

# 傳輸參數
uart_port = "COM3"  # 根據實際情況設定串口
baud_rate = 9600    # UART 波特率
send_grayscale_via_uart(grayscale_values, uart_port, baud_rate)
