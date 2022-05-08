
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

---

### Información adicional para facilitar la corrección

- ¿?
- ¿?
- ¿?
