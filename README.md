# MiniNET

## Presentación de modelo

"...concepto de “Red galáctica”. [Imaginó un conjunto de ordenadores interconectados globalmente, a través de los que todo el mundo podría acceder rápidamente a datos y programas desde cualquier sitio](https://thingspeak.com/channels/141411)..."

## Comunicarse usando paquetes en vez de circuitos

### RED CONMUTADA POR CIRCUITOS

Para establecer comunicación se debe efectuar una llamada y cuando se establece la conexión, los usuarios disponen de un enlace directo a través de los distintos segmentos de la red

#### PASOS PARA ESTABLECER LA CONEXION

A) Establecimiento del circuito:

El nodo EMISOR solicita a un cierto nodo CENTRAL el establecimiento de conexión hacia otro nodo RECEPTOR. Otros nodos pueden intervenir en el establecimiento del circuito.

Este circuito se conoce como CANAL DEDICADO.

B) Transferencia de datos:

El nodo EMISOR envia datos al nodo CENTRAL quien los deriva hacia el otro nodo RECEPTOR sin demoras de nodo a nodo.

C) Desconexión del circuito:

El nodo emisor o el receptor indican a su nodo más próximo que ha finalizado la conexión, y este nodo informa al siguiente de este hecho y luego LIBERA EL CANAL DEDICADO.

### RED CONMUTADA POR PAQUETES

Un paquete es un grupo de información que consta de dos partes:

a) Los datos propiamente dichos y

b) la información de control, que especifica la ruta a seguir a lo largo de la red hasta el destino del paquete.

La conmutación de paquetes permite hacer un uso mas eficiente de CANALES DEDICADOS para datos que pueden ser enviados con retardo en la transmisión (no en tiempo real), tales como el correo electrónico, paginas web, archivos, etc.

Estos son factores que ocasionan que los paquetes tomen rutas distintas para llegar a su destino. Por eso se dice que la ruta que toman los paquetes es "probabilística", mientras que en la conmutación de circuitos, esta ruta es "determinística".

DIFERENCIAS

La conmutación de circuitos es un tipo de comunicación que establece o crea un canal dedicado (o circuito) durante la duración de una sesión.

Los sistemas de conmutación de circuitos son ideales para comunicaciones que requieren que los datos/información sean transmitidos en tiempo real(*).

En sistemas basados en conmutación de paquetes la información/datos a ser transmitida previamente es ensamblada en paquetes. Cada paquete es entonces transmitido individualmente y éste puede seguir diferentes rutas hacia su destino. Una vez que los paquetes llegan a su destino, los paquetes son otra vez re-ensamblados. 

## Medios compartidos y modulacion

## MANCHESTER

## Modelos OSI y TCP-UDP/IP

## Libreria RH
