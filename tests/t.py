class Calculator:
  def add(self, a, b):
    return a + b

#python comment
if __name__ == "__main__":
  calc = Calculator()
  a, b = 10, 3
  print(f"Add: {a} + {b} = {calc.add(a, b)}")