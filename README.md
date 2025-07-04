<p align="center">
  <img src="img/logo.jpg" alt="Logo Subnautica" width="300"/>
</p>

# 🤖 Projeto: Aquário Automático

Este projeto tem como objetivo automatizar o controle de um aquário, integrando alimentação, temperatura, oxigenação e monitoramento de dados em tempo real.

## 🔧 Tecnologias Utilizadas

- [ESP32](https://www.espressif.com/en/products/socs/esp32)
- [Arduino Uno](https://www.arduino.cc/en/Main/ArduinoBoardUno)
- [Arduino IDE](https://www.arduino.cc/en/software) (para programação do Arduino Uno e ESP32)
- Motor de passo
- Sensor de temperatura (DS18B20)
- Aquecedor (SOBO HS-100)
- Resfriador (ventoinha de 5V com controle via ESP32)
- Mini Bomba (JT100)
- Powerbank (fonte de alimentação)
- Impressão 3D personalizada
- Flask + HTML (interface web)
- ScadaBR (visualização industrial dos dados)

## 📦 Funcionalidades

- 🕒 Alimentador automático com horários programados
- 🌡️ Controle de temperatura (aciona aquecedor ou ventoinha automaticamente)
- 💨 Bomba de oxigenação acionada por horários ou sensores
- 📈 Exibição de dados em tempo real via página HTML ou sistema SCADA
- 🔧 Estrutura impressa em 3D para otimizar o espaço do aquário

## 🖥️ Monitoramento com SCADA

- 🖲️ Integração com **ScadaBR** para visualização gráfica e industrial dos dados.
- 📊 Comunicação via protocolo HTTP com servidor Flask intermediando a coleta.
- 🔍 Monitoramento de: temperatura, status do aquecedor, ventoinha, motor do alimentador, bomba de oxigênio e registro da última alimentação.

## 💡 Como usar

1. Clone este repositório:
   ```bash
   git clone https://github.com/SeuUsuario/SeuRepositorio.git
