# LASC-2022
## Repositório utilizado para a LASC 2022

Pastas:

1. Altímetro:
  Nessa pasta está o instalador do software mDACS utilizado para a configuração do altímetro
2. ATOM: 
  Códigos utilizados no ATOM
  * ATOM TTGO LORA SENDER
    Refere-se ao código do TTGO responsável pela aquisição de dados e envio através da telemtetria.
    Utiliza o TTGO conectado a um sensor GY-80 e um GPS NEO-6M
    Faz a leitura dos seguintes dados:
      Latitude, Longitude, Temperatura, Pressão e Altitude
    Armazena todos esses dados em uma única variável iniciada e finalizada por um asterisco(*) e separa os dados através de ponto e vírgula (;)
    Envia através do rádio LORA essa variável que é recebida através do código Lora Reciever TTGO
  * Lora Reciever TTGO
    Refere-se ao código do TTGO utilizado na base de lançamento para o recebimento dos dados enviados
3. Bibliotecas Arduino:
  Todas as bibliotecas utilizadas no projeto, tanto para os códigos de teste quanto para os códigos finais
4. Códigos Teste:
  Todos os códigos utilizados para teste dos componentes de forma separada
5. CP:
  Código final utilizado no Canalle Platinado IV
  Utiliza o sensor GY-87 e um Módulo Micro-SD junto a um Arduino Nano para aquisição e armazenamento dos dados.
6. DataSheets: 
  DataSheets importantes dos módulos utilizados no projeto
7. diagrama eletrico
  Diagramas Elétricos do ATOM e do Canalle Platinado
