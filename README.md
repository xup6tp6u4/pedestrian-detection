# Pedestrian-identification

### 1. 專案概述  
本專案以 FPGA + 軟體的方式，透過無線傳輸及DDR讀取，完成即時的行人偵測。整體流程包括：  
1. 影像擷取：使用 IP CAM 拍攝影像。  
2. 無線通訊：透過 Transmitter (AWV365) 與 Receiver (AWV366) 傳遞影像資料。  
3. 影像處理 & 恢復：在 PC上進行影像讀取與前處理。  
4. 資料傳送至 FPGA：使用 UART 或 AXI 方式，將影像資料載入至 FPGA 的記憶體 (DDR)。   
5. FPGA 偵測流程：  
  影像讀寫 (DDR)  
  特徵提取 (HOG)  
  分類器 (SVM)  
  結果輸出 (VGA OUT )  
6. 顯示結果：將偵測結果在螢幕上做即時標註或輸出。

### 2.系統架構  
![image](https://github.com/user-attachments/assets/f51e8a6d-e662-4d8b-9e00-1b35888fd399)  

### 3.功能與流程    
1. 影像來自 IP CAM，經由 A/V Sender TX 傳輸FM訊號至 A/V Sender RX  
2. A/V Sender RX進入 TVP5150 進行 BT656 訊號解碼 (轉換為 YUV422)，然後通過 AXI FIFO 將影像數據傳遞到 AXI DMA
3. 再經由 DDR Controller 寫入 DDR 作為行人偵測的輸入資料  
4. DDR讀取資料，透過 UART 傳輸至PC，利用python端將影像縮放為 64×128，透過 UART 傳輸至FPGA的DDR  
5. FPGA中的DDR讀取資料，進行行人偵測 (HOG + SVM)  
6. 輸出結果  
若偵測到行人，則以O/X顯示於螢幕 (VGA)  
    * O(判斷為行人)
    * X(判斷不為行人)


### 4.程式碼說明
##### Uart data write ddr.C
* 使用xinlinx sdk將Uart介面做為接收端，將python所發送的灰階值接收，接收的資料利用迴圈將其存入ddr    
##### resize img and uartsend.py 
* 將原圖進行灰階處理，並將灰值圖縮放成64x128的大小，將64x128的灰值圖pixel以8bit的形式通過Uart傳進fpga做接收  
##### 其餘程式碼為實驗室IP，此處不公布 
