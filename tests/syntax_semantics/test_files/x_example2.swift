// 0
// Program 2: Vypocet faktorialu (rekurzivne)

// Hlavni telo programu
// write("Zadejte cislo pro vypocet faktorialu: ")
// let inp = readInt()

// pomocna funkce pro dekrementaci celeho cisla o zadane cislo
func decrement(of n: Int, by m: Int) -> Int {
	return n - m 
}

// Definice funkce pro vypocet hodnoty faktorialu
func factorial(_ n : Int) -> Int {
	var result : Int?
	if (n < 2) {
		result = 1
	} else {
		let decremented_n = decrement(of: n, by: 1)
		let temp_result = factorial(decremented_n)
		result = n * temp_result
	}
	return result!
}

// pokracovani hlavniho tela programu
var end : Int? = 0

while end == 0 {
	write("Zadejte cislo pro vypocet faktorialu: \n")
	let inp = readInt()

	if let inp {
		if (inp < 0)	{ // Pokracovani hlavniho tela programu
			write("Faktorial nelze spocitat!\n")
		} else {
			let vysl = factorial(inp)
			write("Vysledek je: ", vysl, "\n")
			end = 1
		}
	} else {
		write("Chyba pri nacitani celeho cisla!", "\n")
	}
}