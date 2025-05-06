class Product {
  constructor(name) {
    this.name = name;
    this.createdAt = new Date();
  }
}

function Hola() {
  console.log("Hola")
}

const laptop = new Product("Laptop");

Hola();