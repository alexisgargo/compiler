class SymbolTable:
    def __init__(self):
        self.entries = []
        self.lexeme_to_index = {}

    def insert(self, lexeme):
        if not lexeme.strip():
            return -1
        
        if lexeme not in self.lexeme_to_index:
            index = len(self.entries)
            self.entries.append(lexeme)
            self.lexeme_to_index[lexeme] = index
        return self.lexeme_to_index[lexeme]

    def __str__(self):
        return '\n'.join(f"{i}: {entry}" for i, entry in enumerate(self.entries))
