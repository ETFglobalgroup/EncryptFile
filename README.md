# EncryptFile  

---
## 關於 About
使用金鑰來加密或解密檔案
Use key to encrypt or decrypt files.

---
## 指令 Command

### 建立新金鑰 Create new key file
` -g [filename]`

### 加密檔案 Encrypt file
` -e [filename]`

### 解密檔案 Decrypt file
` -d [filename]`

### 設定輸出檔案名稱 Set output file name
` -o [filename]`

### 設定金鑰 Set key file
` -k [filename]`

### 設定載入區塊大小 Set chunk size
區塊大小會影響到速度快慢  
Chunk size would affect execution performance  
` -c [chunk size]`

---
## 範例 Example

### 加密 Encrypt
` encryptfile -e [InputFile] -o [OutputFile] -k [KeyFile]

### 解密 Decrypt
` encryptfile -d [InputFile] -o [OutputFile] -k [KeyFile]