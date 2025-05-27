from transition_table import DFA, transform, ERROR, ACCEPT
from tokens import TOKENS
from symbol_table import SymbolTable

def is_accept(state):
    return state in (ACCEPT)

def is_error(state):
    return state == ERROR

def is_delimiter(ch):
    return ch in "(){}[]@;:,."

def analyze_file(filename):
    with open(filename, 'r') as file:
        content = file.read()

    i = 0
    state = 0
    tokens = []
    symbol_table = SymbolTable()

    while i < len(content):
        state = 0
        lexeme = ""

        while i < len(content) and not is_accept(state) and not is_error(state):
            #print("Leyendo...")
            ch = content[i]
            #print(ch)
            col = transform(ch)

            if col == -1:
                #print("Transition Table -1 col error")
                state = ERROR
                break

            state = DFA[state][col]
            #print("Next State: " + str(state))
            if state == ERROR:
                #print("Error state Reached from Transition Table")
                break

            if is_delimiter(ch):
                #print("Delimeter reached!")
                if lexeme:
                    break   
                else:
                    lexeme = ch
                    i+=1
                    break

            lexeme += ch
            i+= 1
            

        if is_accept(state):
            lexeme = lexeme.strip()
            if not lexeme:
                continue
            
            #print("<" + lexeme + ">")
            if lexeme in TOKENS:
                token_id = TOKENS[lexeme]
                entry = -1
            else:
                token_id = 100
                entry = symbol_table.insert(lexeme)

            tokens.append((token_id,entry))
        
        else:
            i+=1

    return tokens, symbol_table

if __name__ == "__main__":
    tokens, symbol_table = analyze_file("input.txt")
    for t in tokens:
        print(f"<{t[0]}, {t[1]}>")
    print("\nSymbol Table:")
    print(symbol_table)