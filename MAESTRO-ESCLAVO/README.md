# RESUMEN

Red dispuesta en el modo MAESTRO-ESCLAVO y un GATEWAY que se encarga de subir los datos a un servicio en la nube.

El arbitraje para el uso del canal de comunicacion (RF 433Mhz) se implementa por medio de un mensaje a cada esclavo.

Una vez recibido, cada esclavo responde con la estructura predefinida.

El MAESTRO, a la salida del puerto serie, transmite una cadena compuesta por el numero asignado a cada esclavo, seguido del valor sensado y separados por un caracter ",".

El GATEWAY es un programa escrito en PROCESSING que, una vez que abre el puerto serie, convierte la cadena al formato de la API del Servicio THINGSPEAK.
