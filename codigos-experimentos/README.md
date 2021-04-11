# Experimentos para validação da solução de atualização de firmware e da respectiva implementação 

Neste repositório estão os códigos desenvolvidos em `Python 3` que eram responsáveis pela comunicação e transmissão do *firmware* do transmissor para o dispositivo que seria atualizado. 

O primeiro experimento teve como objetivo realizar uma atualização de *firmware* de forma correta e o comportamento do dispositivo e do transmissor pode ser encontrado [nesta](diagramas/digrama-atv-experimento1.pdf) figura. Esse experimento utilizou o código [`atualizacao_bem_sucedida.py`](testes/atualizacao_bem_sucedida.py).

O segundo e terceiro experimentos tiveram como objetivo realizar uma atualização de *firmware* transmitindo para o dispositivo uma versão de *firmware* inferior e uma assinatura inválida e o comportamento do dispositivo e do transmissor pode ser encontrado [nesta](diagramas/digrama-atv-experimento23.pdf) figura. Esses experimentos utilizaram os  códigos [`fw_inferior.py`](testes/fw_inferior.py) e [`assinatura_errada.py`](testes/assinatura_errada.py), respectivamente.


O quarto experimento teve como objetivo realizar uma atualização de *firmware* enviado ao dispositivo um resumo criptográfico errado. O comportamento do dispositivo e do transmissor pode ser encontrado [nesta](diagramas/digrama-atv-experimento4.pdf) figura. Esse experimento utilizou o código [`resumo_errado.py`](testes/resumo_errado.py).

O último experimento teve como objetivo interromper a atualização de *firmware* no meio do processo. O comportamento do dispositivo e do transmissor pode ser encontrado [nesta](diagramas/digrama-atv-experimento5.pdf) figura. Esse experimento utilizou o código [`transferencia_interrompida.py`](testes/transferencia_interrompida.py).

Uma descrição mais detalhada dos experimentos pode ser encontrada na [monografia](../monografia/monografia.pdf).





