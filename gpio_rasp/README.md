# Introducción a los Sistemas Embebidos, Proyecto 1, Smart House
Instituto Tecnológico de Costa Rica  
Ingeniería en Computadores.  
2019, II Semestre.  

## Biblioteca para manejo de GPIO de Raspberry pi 2

### Instrucciones
1. Abrir terminal dentro de carpeta raíz.
2. Ejecutar el comando:
```
autoreconf --install
```
3. Crear carpeta "build" dentro de carpeta raíz
4. Ingresar a build y crear carpeta "usr" dentro de ella
5. Dentro de la carpeta "build", ejecutar comando: 

```
source /opt/poky/2.5.1/environment-setup-cortexa7hf-neon-vfpv4-poky-linux-gnueabi
```

Nota: Tomar en cuenta que la ruta dada (/opt/poky/2.5.1/..) puede variar en cada máquina, dependiendo de la versión de Yocto instalada)

6. Dentro de la carpeta build ejecutar comando:
```
../configure --host=arm-poky-linux-gnueabi --prefix=/home/jerodso/gpio_rasp/build/usr
```
7. Dentro de la carpeta build ejecutar comando: 
```
make
```
8. Dentro de la carpeta build ejecutar comando:
```
make install
```
9. El archivo ejecutable se encuentra dentro de "<carpeta_raíz>/build/usr/bin"

## Autor
* Jeremy Rodríguez Solórzano  
201209131  
jrodriguezs0292@gmail.com

## Version
1.0.0
