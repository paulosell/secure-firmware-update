from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.serialization import Encoding, PrivateFormat, NoEncryption
import hashlib
import math
import serial
import sys 

def update(firmware,v, assinatura_falsa=False):
    if (firmware > 2 and firmware < 1):
        return print("Modelo inválido")

    if firmware == 1:
        with open('../tcc-implementacao-tz_NonSecure_v1.bin', 'rb') as file:
            fw = file.read()
    if firmware == 2:
        with open('../tcc-implementacao-tz_NonSecure_v2.bin', 'rb') as file:
            fw = file.read()

    with open('../chave_privada.pem', 'rb') as file:
        chave_privada = serialization.load_pem_private_key(file.read(), password=None)
   
    v = v.to_bytes(8, byteorder='big')
  
    size= len(fw)
    size = size.to_bytes(8, byteorder='big')
 

    quadros = []
    
    num_quadros = math.floor(len(fw)/480)
    for i in range(num_quadros):
        quadros.append(fw[i*511:(i+1)*480])
    mod = len(fw) % 480
    quadros.append(fw[num_quadros*480:num_quadros*480+mod])
    fw_sig = chave_privada.sign(fw+v, padding.PKCS1v15(), hashes.SHA256())
    
    assets = fw_sig+size+v
    T = len(assets) 
    T = T.to_bytes(4, byteorder='big')
    assets = bytes([4]) + T + assets
    ser = serial.Serial('COM21')
    print("Iniciando upload do novo firmware...")
    ser.write(assets)
     
    total = 100/len(quadros)
    completo = 0
    print(f"Completado: {completo}%")
    for quadro in quadros:
        T = len(quadro)
        T = T.to_bytes(4, byteorder='big')
        codigo = bytes([5]) + T + quadro
        ser.write(codigo)
        flag = 1
        while flag==1:
            r = ser.read(1)
            if r==bytes([0]):
                flag = 0
                completo = completo + total
                print(f"Completado: {completo}%")
    
    
    print("Transmissão finalizada")
    fim = bytes([6]) 
    T = 1
    T = T.to_bytes(4, byteorder='big')
    
    (ser.write(fim+T))

    

modelo  = 1
assert modelo == 1 or modelo == 2, print("Modelo deve ser 1 ou 2")
versao_fw = 1
assert versao_fw > 0, print("Versão deve ser maior que 0")

update(modelo,versao_fw, assinatura_falsa=False)