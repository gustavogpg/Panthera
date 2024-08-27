# Projeto de Sensor de Suspensão

Este projeto consiste em um código para leitura de um sensor de potenciômetro linear instalado na suspensão de uma moto, desenvolvido pelo Lucas Bosso de Mello e Gustavo Garcia no dia 25/08/2024.

## Descrição do Projeto

Ao contrário do sensor de combustível, o sensor de suspensão opera de maneira ideal, fornecendo leituras suaves e consistentes. O gráfico gerado pelo SerialPlot mostra uma curva completamente suave, sem necessidade de qualquer tratamento adicional de dados.

### Funcionamento Perfeito

O sensor de suspensão é altamente estável e preciso, e como tal, não foi necessário aplicar filtros ou métodos de suavização. A leitura direta do sensor reflete fielmente as condições da suspensão, garantindo a confiabilidade do sistema.

### Imagens do Projeto

- Foto do sensor de suspensão (ainda falta tirar a foto do sensor):

<p align="center">
  <img src="" alt="Sensor da suspensão" width="600" style="display: block; margin-left: auto; margin-right: auto;"/>
</p>

<br>

- Gráfico obtido no Serial Plot:

<p align="center">
  <img src="serialplot.jpg" alt=""Foto do sensor funcionando no SerialPlot" width="600" style="display: block; margin-left: auto; margin-right: auto;"/>
</p>

## Código-Fonte - [potenciometro_linear_suspensao.ino](https://github.com/gustavogpg/Panthera/blob/75c0a8f8b03a4afc2f01727711bb2114db5e7e05/arduino/sensores/sensor%20da%20suspens%C3%A3o/potenciometro_linear_suspensao.ino)

```cpp
//------------------------------------------------------------
/* EESC - USP Guepardo / Lucas Bosso de Mello + Gustavo Garcia
   Código para leitura do potenciômetro linear da sensor do tanque de combustível
   25/08/2024
*/ 

# define ADC 0 // porta analógica A0

// ------------------------------------------------------------------------
// Configuração GPIO , periféricos, etc:

void setup ( void ) 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin ( 9600 );

  // Configuração do ADC:
  analogReference ( DEFAULT ) ; // faixa de tensões elétricas de entrada : 0 V − 5 V
 }

// ------------------------------------------------------------------------

// Loop principal
void loop ( void ) 
{
// ------------------------------------------------------------------------
 
  // Declaração das variáveis
  int leitura = 0; // leitura analógica
  int deformacao = 0; // leitura mapeada
  int valor_tanque = 0; // leitura do tanque de 0 a 100
  int porcentagem_tanque = 0; // porcentagem tanque limitada de 0 a 100


// ------------------------------------------------------------------------

  // Leitura da tensão analógica no pino A0:
  leitura = analogRead (ADC); // analogRead ( pin );
  valor_tanque = map(leitura,783,993,0,100);

  // Limitar a porcentagem para garantir que está entre 0% e 100%
  porcentagem_tanque = constrain(valor_tanque, 0, 100);

  // ------------------------------------------------------------------------
  // Imprimir leitura na porta serial:
  Serial.println(porcentagem_tanque);
  delay(10);
}

