/*Vysledek je: 0x1.3bp+15
*/
/*8*/

let a : Int? = readInt()
if let a {
    if (a < 0) {write("Faktorial nelze spocitat\n")
    } else {
        var a = Int2Double(a)
        var vysl : Double = 1
        while (a > 0) {
            vysl = vysl * a
            a = a - 1
        }
        write("Vysledek je: ", vysl, "\n")
    }
} else {
    write("Chyba pri nacitani celeho cisla!\n")
}