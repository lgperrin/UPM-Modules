# Información sobre el fichero `docker-compose.yml`

## ¿Qué es este fichero?

Este fichero `docker-compose.yml` no es más que la plantilla que considero necesaria como para poder
desplegar la herramienta de Mage AI junto con los escenarios requeridos para el TFG propuesto. En él,
se definen una serie de servicios que se "comunican" a través de una red (_network_).

## Explicación del Fichero `docker-compose.yml`

* **Versión de Docker Compose**: `3.8`
  - Especifica la versión de Docker Compose utilizada.

* **Servicios Definidos**
  * Servicio PostgreSQL. Usa la imagen de PostgreSQL versión 13 basada en Alpine Linux 3.17.
    - El contenedor se nombra como `postgres-db`.
    - El contenedor se reiniciará automáticamente si se detiene `always`.
    - Utiliza el contenedor `./postgres/pgdata:/var/lib/postgresql/data`
    - Persistencia de datos de PostgreSQL en `./postgres/pgdata`.
    - `./postgres/init-magedb.sh:/docker-entrypoint-initdb.d/init-magedb.sh`
    - _Script_ personalizado para inicializar la base de datos.
    - El puerto 5432 del contenedor se mapea al puerto 5432 del _host_ como `5432:5432`.
    - Se hace uso de las variables de entorno `POSTGRES_USER`, `POSTGRES_PASSWORD`, `POSTGRES_DB`
      para configurar los credenciales y la base de datos por defecto.
      
  * Servicio Mage AI. Usa la imagen `mageai/mageai:latest` que es la última versión de Mage AI.
    - El contenedor se nombra como `mage-ai`.
    - El contenedor se reinicia hasta 5 veces si falla `on-failure:5`.
    - Hace uso del comando `"/app/run_app.sh mage start demo"` para ejecutar el _script_ que inicializa Mage AI en modo _demo_.
    - El volumen `./workspace/demo:/home/src` mapea el directorio de trabajo para la persistencia de datos.
    - El puerto 6789 del contenedor se mapea al puerto 6789 del _host_ como `6789:6789`.
    - Se hace uso de la variable de entorno `MAGE_DATABASE_CONNECTION_URL` que define la URL de conexión a la base de datos
      PostgreSQL.

  * **Redes**. La red predeterminada es `mage` que se utiliza como una red externa de conexión entre los dos servicios definidos
  con anterioridad.
