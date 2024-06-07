# Autora: Laura García Perrín
# Fecha: 13/05/2023
# Preámbulo: añadir librerías y paquetes necesarios
# Página web de referencia: https://www.christophenicault.com/post/improve_shiny_ui/

library(shiny)
library(ggplot2)
library(shinythemes)

ui <- fluidPage (
  theme = shinytheme("cerulean"),
  titlePanel(
    fluidRow(
      column(9, h1("Data visualization"), h4("This is a Shiny app whose autor is Laura García Perrín. 
                                             In here, you can see the scatterplot and histogram of
                                             the mtcars dataset.")), 
      column(3, img(src="logoupm.jpg", width=200))
      )
    ),
  tabsetPanel(
    tabPanel(title = "Scatterplot",
             sidebarPanel(
               selectInput(inputId = "attr1",
                           label = "Atributo para el eje X",
                           choices = names(mtcars)),
               selectInput(inputId= "attr2",
                           label = "Atributo para el eje Y",
                           choices = names(mtcars)),
               selectInput(inputId= "attr3",
                           label = "Tama~no de las variables",
                           choices = names(mtcars)),
               selectInput(inputId= "attr4",
                           label = "Color de las variables",
                           choices = names(mtcars))
             ),
             mainPanel(plotOutput("scatterPlot"))
             ),
    tabPanel(title = "Histograma", 
             sidebarLayout(
               sidebarPanel(
                 selectInput(inputId = "x",
                             label = "Variable en el eje X",
                             choices = names(mtcars)),
                 sliderInput(inputId = "bins",
                             label = "Número de bins",
                             min = 2,
                             max = 50,
                             value = 5)
               ),
               mainPanel(plotOutput("histogram"))
             )
            )
  )
)

server <- function(input, output) {
  output$scatterPlot <- renderPlot({
    datos <- mtcars[, c(input$attr1, input$attr2, input$attr3, input$attr4)]
    datosProc <- datos
    datosProc[,input$attr4] <- as.factor(datosProc[,input$attr4])
    ggplot(data = datosProc, aes(x=datosProc[,1], y=datosProc[,2],
                                 size=datosProc[,3], color=datosProc[,4])) + 
      geom_point(alpha =0.75) +
      scale_size(range = c(1, 20)) +
      labs(x=colnames(datosProc)[1], y=colnames(datosProc)[2], 
           size=colnames(datosProc)[3], color=colnames(datosProc)[4]) 
  })
  
  output$histogram <- renderPlot({
    x <- mtcars[,input$x]
    bins <- seq(min(x), max(x), length.out = input$bins + 1)
    hist(x, breaks = bins, xlab = input$x, ylab='Frequency',
         main = paste("Histogram of", input$x, sep=" "))
  })
}

shinyApp(ui=ui,server=server)





