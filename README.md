
# IIC2333 - Sistemas Operativos y Redes
## Proyecto 1 - SuperSpeedDrive FileSystem

---

## Grupo
### DCCegmentationFault

|      Nombre      |  Usr. GitHub   |      Correo       | Nro. Alumno |
|------------------|----------------|-------------------|-------------|
| Matías López     | MatLop1        | milopez8@uc.cl    | 17210674    |
| Tomás Cortés     | ticortes       | ticortez@uc.cl    | 17640849    |
| Felipe Villagrán | BibarelUsedFly | fivillagran@uc.cl | 16638689    |
| Marcelo Bernal   | msbernal       | msbernal@uc.cl    | 1763671J    |
| Luis González    | ljgonzalez1    | ljgonzalez@uc.cl  | 16625439    |

---

### Principales decisiones de diseño para construir el programa

- Si se abre un archivo con write y ya existe, se retorna NULL
- Si se está escribiendo y la escritura causará un rotten, se detiene el proceso ya que se debería haber manejado con os_trim y esto no debería pasar, o cuando comienze pasar debería cambiarse el disco ya que todos los bloques comenzarán a fallar.
- Para abrir un nuevo disco se tienen que haber cerrado todos los archivos


---

### Supuestos adicionales ocupados

- El bloque 4 correspondiente al directorio, dice 0 en empty, pero no puede ser escrito porque es el bloque de directorio
- Un bloque con una o más páginas *rotten* se convierte en un bloque ilegible puesto a que nunca debió ser escrito en primer lugar por restricciones ya existentes.

---

### Información adicional para facilitar la corrección

- Se crea el módulo `src/ssdfs/debug/debug.h` para printear mensajes y añadir pausas.
  - No es la idea, pero se puede cambiar el parámetro `DEBUG_MODE` de `false` a `true` para mostrar mensajes adicionales (si es que aún siguen en la versión de entrega).
  - En caso de que `DEBUG_MODE` sea `true`, el parámetro `SIMULATE_WAIT` indica que los mensajes de debug con pausa harán la pausa efectivamente.
  - Obviamente requiere recompilar el programa.
