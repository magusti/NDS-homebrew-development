
<h1>Actualización del tutorial How to create a Bouncing Ball"</h1>
<p>Actualización del tutorial original de M. Jonhson [<a href="tut_original">1</a>] y PFC de J. D. Jaén [<a href="pdf">2</a>]</p>

<h2>Organización documental</h2>
<p>Estos documentos explicarán cómo se han realizado las diferentes subversiones del directorio "bbal_versions"
<ul>
 <li>gestioNivellsNintendoNDS.pdf</li>
 <li>gestioSpritesNintendoNDS.pdf</li>
 <li>gestioSonitNintendoNDS.pdf</li>


<h2>Organización de directorios</h2>
 <p>Estás son las versiones que puedes ver en este recopilatorio. En la medida de lo posible se han conservado los comentarios de los autores originale.

 <pre>
 tutorial_BouncingBall -.-> directorio base
   |- bball_tutOriginal   -.-> versión original (incompleta, falta el audio). Veasé [<a href="tut_original">1</a>].
   |- bball_tutJaen  -.-> 1a versión revisada (anotada, completada y ampliada). Veasé [<a href="pdf">2</a>].
   |- bball_vFinal --> completa.
   |- bball_versions -.-> versiones intermedias en orden cronológico.
    |- bball_subVersionsBackgrounds
     |- bball__v01 -.-> Es la incial, muestra la carga de recursos y la generación del fondo de cielo liso.
     |- bball__v02 -.-> Muestra la carga de recursos, cómo se genera el cielo con su banda de “nubes“ y el suelo (o la plataforma) sobre el que se limitará el movimiento de los objetos. Es la versión reconstruida sobre el texto del tutorial original [1], se puede ver un gran número de instrucciones que hacen referencias a registros.
     |- bball__v03 -.-> Es la versión de v02, con los elementos estáticos de la escena terminados, solo que aquí se muestran las simplificaciones y normalizaciones de código que hemos introducido.
        Se observará el cambio de instrucciones que hacen referencia a registros en la v02 por instrucciones de la versión actual del SDK. A partir de esta versión, y con la misma idea de actualizar y simplificar el código se realizarán las siguientes versiones.
    |- bball_subVersionsSprites.
     |- bball__v04 -.-> Configuración de memoria y carga de los recursos asociados al gráfico que hace de “pelota”, el sprite.
     |- bball__v05 -.-> Generar 51 sprites, utilizando el inicial para generar copias de él, que se moverán aleatoriamente por la pantalla.
        Se incorpora la interacción del usuario con la aplicación, mediante el uso de la botonera de la NDS, así que hay una pelota que si "controlamos".
        En esta versión se hace uso de las instrucciones como se puede ver en el tutorial original.
     |- bball__v06 -.-> bball__v06. Volviendo a la versión v04 de un solo sprite", se incorpora la interacción del usuario con la aplicación, mediante el uso de la botonera de la NDS.
       Se observará en esta versión el cambio de instrucciones que hacen referencia a registros en la v04 y vp5 por instrucciones de la versión actual del SDK, con la idea de actualizar y simplificar el código. También se verá un cambio en los colores del suelo ... Me apetecía.
    |- bball_subVersionsSonido
     |- bball__v07 -.-> Incluye el audio que se utiliza como banda sonora (sonidos/bsmusic.mod).
     |- bball__v08 -.-> Incluye solo los efectos (FX) de sonido (sonidos/boing.wav).
 </pre>

   
   
<h2>Bibliografía</h2>
 <p><a name="tut_original">[1] “How to Make a Bouncing Ball Game for Nintendo DS” <http://ekid.nintendev.com/bouncy/>. Este trabajo ya no está disponible en la red; pero existe una copia de fecha de 2015 en el directorio "bball_tutOriginal" que es al que nos referiremos al hablar del “tutorial original”.</a></p>

<p><a name="pfc"> [2] J. D. Jaén. (2015). Tutorial práctico para desarrollo de videojuegos sobre plataforma Nintendo
NDS. Disponible en <http://hdl.handle.net/10251/56433>.</a></p>
