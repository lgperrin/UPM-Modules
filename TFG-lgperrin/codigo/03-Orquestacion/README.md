# Orquestación y Machine Learning en los Data Pipelines

## Data pipelines de aprendizaje automático

Hola a todos, bienvenidos al módulo sobre pipelines de aprendizaje automático. En este módulo, vamos a tomar un cuaderno de Jupyter que hicimos anteriormente, lleno de código y experimentos, y lo vamos a convertir en algo que podamos ejecutar fácilmente, algo reproducible y manejable.

## ¿Qué es un pipeline de aprendizaje automático?

Un pipeline de aprendizaje automático es simplemente una serie de pasos que seguimos para entrenar un modelo. Piensa en ello como una receta de cocina: tienes que seguir varios pasos en orden para obtener el resultado final.

## Pasos del pipeline

1. **Descargar los datos**: Este es el primer paso, donde obtenemos los datos que necesitamos. Es como conseguir todos los ingredientes para nuestra receta.

2. **Transformar los datos**: Aquí, preparamos los datos para que sean útiles. Esto puede incluir limpiar los datos, convertir formatos o eliminar información irrelevante, como preparar y cortar los ingredientes antes de cocinarlos.

3. **Preparar los datos para el aprendizaje automático**: En este paso, hacemos que los datos estén listos para ser usados por el modelo. Esto puede incluir crear nuevas características a partir de los datos existentes, similar a mezclar y combinar ingredientes para crear una masa.

4. **Ajuste de hiperparámetros**: Aquí, buscamos los mejores parámetros para nuestro modelo, como ajustar la temperatura y el tiempo de cocción en nuestra receta para obtener el mejor resultado.

5. **Entrenar el modelo**: Usamos los parámetros que encontramos en el paso anterior para entrenar el modelo, es decir, hacemos que aprenda de los datos. Es como poner la mezcla en el horno y dejar que se cocine.

## Desafíos y Soluciones

Un cuaderno de Jupyter puede volverse muy desordenado y difícil de manejar, especialmente si necesitas ejecutar todo en orden cada vez. Es muy largo, y si necesitamos volver a ejecutarlo, tendríamos que abrirlo en nuestra máquina local y ejecutar las celdas en orden. También, hay muchas cosas que no sabemos qué hacen, especialmente si no somos quienes creamos este cuaderno. Para resolver esto, podemos convertir nuestro cuaderno en un script de Python. Sin embargo, esto también tiene sus limitaciones, como la programación de tareas y la colaboración en equipo.

## Orquestación de flujos de trabajo

Para manejar estos desafíos, usamos herramientas de orquestación de flujos de trabajo como Airflow, Prefect o Mage. Estas herramientas nos ayudan a organizar y ejecutar nuestros pasos de manera ordenada y eficiente. Funcionan como un chef profesional que coordina todo en la cocina para asegurarse de que cada paso se complete correctamente y a tiempo. Usar estas herramientas nos permite mantener nuestros procesos organizados, manejables y escalables, lo que es crucial a medida que nuestros proyectos crecen y se vuelven más complejos. En este módulo, aprenderemos cómo implementar estos pipelines de aprendizaje automático nosotr@s mism@s.

## Ejecutar la herramienta (Mage) con Docker

Una forma de ejecutar herramientas es a través de Docker, para lo cual necesitaremos tener Docker y Docker Compose instalados. 

### Verificar instalación de Docker y Docker Compose

Primero, nos aseguramos de que Docker esté instalado. Ejecutamos el siguiente comando desde nuestro terminal:
```bash
docker run hello-world
```
Este comando debería ejecutarse correctamente si Docker está instalado. También deben tener Docker Compose, pueden verificarlo con:
```bash
docker-compose --version
```

### Clonar el repositorio y configurar el entorno

1. **Clonar el repositorio**: Vayan a la carpeta de orquestación y clonen el repositorio necesario. Cambien al directorio del proyecto clonado:
   ```bash
   git clone https://github.com/your-repo/ mlops
   cd mlops
   ```

2. **Configurar el proyecto**: Abran el proyecto en Visual Studio Code para revisar y editar los archivos necesarios. El archivo `start.sh` es crucial ya que establece variables y ejecuta Docker Compose.

3. **Modificar `start.sh` para Windows (opcional)**: Si están en Windows, puede que necesiten ajustar algunos comandos para evitar problemas con las rutas relativas. Por ejemplo, reemplazar los puntos (`.`) con `$(pwd)` y poner las rutas entre comillas.

### Ejecutar el script de inicio

1. **Crear archivo `gitconfig` vacío**: Si no tienen un archivo `gitconfig`, creen uno vacío para evitar errores:
   ```bash
   touch ~/.gitconfig
   ```

2. **Ejecutar el script de inicio**:
   ```bash
   ./scripts/start.sh
   ```
   Esto ejecutará Docker Compose y configurará todo el entorno necesario. Este proceso puede tardar unos minutos.

### Acceder a Mage

Una vez que todo esté configurado, podemos acceder a Mage abriendo un navegador web y escribiendo la ruta `localhost:6789`. 

### Probar los cambios

Para asegurarse de que los cambios que hagan en el navegador se reflejan en su sistema local, pueden editar un archivo, como `init.py`, agregar una línea de prueba (por ejemplo, `print("123")`), y verificar en VS Code si los cambios se reflejan.

### Alternativas para ejecutar Mage

También pueden instalar Mage directamente como un paquete de Python:

1. **Instalar Mage y sus dependencias**:
   ```bash
   pip install mage-ai boto3
   ```

2. **Iniciar Mage**:
   ```bash
   mage start -p ml_ops
   ```
