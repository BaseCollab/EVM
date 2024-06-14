// VM can allocate maximum 32MB for user objects using a single mmap() call
// If benchmark proves to be frontend-heavy, you can implement it using bytecode

// Consider 0 to be default value for number
class Bar {
    public a: number = 0
}

// Consider 0 to be default value for number
// Consider null to be default value for ref types
// Consider any reference type to be nullable, no need to support union types like X | null separately
class Foo {
    public x: number = 0
    public y: Bar | null = null
}

function dump(foo: Foo[]): void {
    for (let i : number = 0; i < foo.length; i++) {
        // This line of code should not be optimized out by you/frontend, leave it as is
        let f = new Foo()

        f = foo[i]

        if (f == null) {
            console.log("Foo:null") // intrinsic print, NOT stdlib call
        } else {
            let b = f.y
            if (b == null) {
                console.log("Foo.Bar:null") // intrinsic print, NOT stdlib call
            } else {
                console.log(b.a) // intrinsic print, NOT stdlib call
            }
        }
    }
}

function foo(N : number, M : number) : void {
    // Do not implement generic types. Just call new array with type descriptor as argument
    let foo : Foo[] = new Array<Foo>(M)
    // Consider any reference type to be nullable, no need to support union types like X | null separately
    let outer : Foo | null = null

    for (let i : number = 1; i <= N; i++) {
        let o1 = new Foo()
        o1.x = i
        if (i % 3 == 0) {
            foo[i % (M - 1)] = o1
        }

        let o2 = new Bar()
        o2.a = i
        if (i % 5 == 0) {
            o1.y = o2
        }

        outer = o1

        dump(foo)
    }
}

function main() : void {
    let N : number = 40000
    let M : number = 1000
    foo(N, M)
}

main()
