# Introducción al TFG

## Evolución del Enfoque Data-centric (1960 – Actualidad)

La gestión de datos ha pasado de centrarse en almacenamiento y procesamiento a un enfoque _data-centric_, considerando los datos como un activo esencial. Esto ha impulsado soluciones de Big Data y la proliferación de _data pipelines_ para la gestión de datos a gran escala. La gestión de datos es estratégica para la toma de decisiones. La construcción de data pipelines eficientes y escalables es esencial para mantener operaciones en un entorno complejo y exigente.

Ahora bien, con el auge del Big Data, las organizaciones deben gestionar enormes volúmenes de datos, enfrentando desafíos en procesamiento, calidad, integridad y seguridad de los datos. La necesidad es desarrollar y mantener _data pipelines_ efectivos y eficientes.

## ¿Qué son los data pipelines?

Son conjuntos de tareas que transforman datos de entrada antes de llegar a un destino. Representados como grafos acíclicos dirigidos (DAG), muestran dependencias entre tareas, cruciales para transformar datos en información procesable.

Por ejemplo, un pipeline de aprendizaje automático es simplemente una serie de pasos que seguimos para entrenar un modelo. Piensa en ello como una receta de cocina: tienes que seguir varios pasos en orden para obtener el resultado final. 

1. **Descargar los datos**: Este es el primer paso, donde obtenemos los datos que necesitamos. Es como conseguir todos los ingredientes para nuestra receta.

2. **Transformar los datos**: Aquí, preparamos los datos para que sean útiles. Esto puede incluir limpiar los datos, convertir formatos o eliminar información irrelevante, como preparar y cortar los ingredientes antes de cocinarlos.

3. **Preparar los datos para el aprendizaje automático**: En este paso, hacemos que los datos estén listos para ser usados por el modelo. Esto puede incluir crear nuevas características a partir de los datos existentes, similar a mezclar y combinar ingredientes para crear una masa.

4. **Ajuste de hiperparámetros**: Aquí, buscamos los mejores parámetros para nuestro modelo, como ajustar la temperatura y el tiempo de cocción en nuestra receta para obtener el mejor resultado.

5. **Entrenar el modelo**: Usamos los parámetros que encontramos en el paso anterior para entrenar el modelo, es decir, hacemos que aprenda de los datos. Es como poner la mezcla en el horno y dejar que se cocine.

## Desafíos y Soluciones

Un cuaderno de Jupyter puede volverse muy desordenado y difícil de manejar, especialmente si necesitas ejecutar todo en orden cada vez. Es muy largo, y si necesitamos volver a ejecutarlo, tendríamos que abrirlo en nuestra máquina local y ejecutar las celdas en orden. También, hay muchas cosas que no sabemos qué hacen, especialmente si no somos quienes creamos este cuaderno. Para resolver esto, podemos convertir nuestro cuaderno en un script de Python. Sin embargo, esto también tiene sus limitaciones, como la programación de tareas y la colaboración en equipo. Así pues, cambia el paradigma de hacerlo todo en local a querer utilizar otras soluciones más colaborativas, escalables y fiables.

## Orquestación de flujos de trabajo

Para manejar estos desafíos, usamos herramientas de orquestación de flujos de trabajo como Airflow, Prefect o Mage. Estas herramientas nos ayudan a organizar y ejecutar nuestros pasos de manera ordenada y eficiente. Funcionan como un chef profesional que coordina todo en la cocina para asegurarse de que cada paso se complete correctamente y a tiempo. Usar estas herramientas nos permite mantener nuestros procesos organizados, manejables y escalables, lo que es crucial a medida que nuestros proyectos crecen y se vuelven más complejos. 


