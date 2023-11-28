/*fail_every_time*/
// this test never succeeds
// it is here for generation purposes
// after you know the work with generation counters is alright, delete this

var a = "var0"

if (true) {
    var a = "if0var1"
    if (true) {
        var a = "if1var2"
    }
}

if (true) {
    var a = "if2var3"
}

func function() {
    var a = "_functionvar0"

    if (true) {
        var a = "_functionif0var1"
        if (true) {
            var a = "_functionif1var2"
        }
    }

    if (true) {
        var a = "_functionif2var3"
    }

    var b = "_functionvar4"

    while(true) {
        var a = "_functionwhile0var5"
    }

    var c = "_functionvar6"

    while(true) {
        var a = "_functionwhile1var7"
        while(true) {
            var a = "_functionwhile2var8"
        }
    }

    while(true) {
        var a = "_functionwhile3var9"
    }

    var d = "_functionvar10"
}

var b = "var4"

while(true) {
    var a = "while0var5"
}

var c = "var6"

while(true) {
    var a = "while1var7"
    while(true) {
        var a = "while2var8"
    }
}

while(true) {
    var a = "while3var9"
}

var d = "var10"
