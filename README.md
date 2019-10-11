# Linha de produção com Arduíno e sensor RFID

## Resumo:

Implementação de uma esteira com um sensor RFID que identifica tags NFC e escolhe se o objeto deve ou não continuar na linha de produção, sendo, dependendo do caso, jogado para fora da linha. 
Esse projeto simula uma linha de produção de industria 4.0 com possibilidade de tracking individual de cada produto.
    
## Materiais utilizados:
* 1 Arduíno Uno
* 3 Motores
* 1 Módulo Leitor Rfid Mfrc522
* 2 Tags Mifare UltraLight
* 1 Tag Mifare One
    
## Descrição de funcionamento:

A esteira, que utiliza dois motores, roda até que um objeto com uma tag passe pelo sensor.
Ao passar pelo sensor, da-se um feedback no monitor serial do arduino a respeito da tag,
dizendo o tipo e o número da tag. A esteira para e o programa verifica se esse objeto deve
continuar na linha ou deve ser jogado para fora. Caso deva continuar, a esteira volta a 
funcionar. Caso deva ser jogado para fora, a esteira anda um pouco para alinhar o objeto
à pá do terceiro motor. O terceiro motor é ligado girando a pá e jogando o objeto para 
fora da linha, e é ligada novamente em sentido contrário para retornar a pá para a posição
inicial. A esteira volta a funcionar e aguardar o próximo objeto

## Vídeo de funcionamento

[https://www.youtube.com/watch?v=fqdGpaWxUo8](https://www.youtube.com/watch?v=fqdGpaWxUo8)

---

Projeto Final de EA076 ministrado na Universidade Estadual de Campinas desenvolvido por Guilherme Nishino Fontebasso (RA 135973) e João Paulo de Tassis Machado (RA 136261). Entregue dia 18.06.2017.
