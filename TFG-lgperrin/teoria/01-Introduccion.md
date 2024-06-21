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


