import gdb


def dimacs_format(internal_representation):
    output = int(internal_representation)
    output += 2
    if output % 2 == 1:
        output -= 1
        output *= -1
    output //= 2
    return str(output)


class ClausePrinter:

    def __init__(self, val):
        self.val = val

    def to_string(self):
        vector_type = self.val['literals'].type.template_argument(0)

        if vector_type is None:
            return "Empty vector"

        # Get the size of the vector
        size = self.val['literals']['_M_impl']['_M_finish'] - self.val['literals']['_M_impl']['_M_start']

        # Create a Python list of the vector elements
        elements = []
        for i in range(size):
            element = self.val['literals']['_M_impl']['_M_start'][i]
            elements.append(str(element))

        elements = list(map(dimacs_format, elements))

        # Return a string representation of the vector elements
        return '{' + ', '.join(elements) + '}'


class WatchlistsPrinter:

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return 123


class ClauseRefPrinter:

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return self.val['_M_data']


def my_pp_func(val):
    if str(val.type) == 'std::vector<std::reference_wrapper<Clause>>':
        return WatchlistsPrinter(val)
    if str(val.type) == 'std::vector<std::vector<std::reference_wrapper<Clause>>>':
        return WatchlistsPrinter(val)
    if str(val.type) == 'std::reference_wrapper<Clause>':
        return ClauseRefPrinter(val)
    if str(val.type) == 'Clause':
        return ClausePrinter(val)



gdb.pretty_printers.append(my_pp_func)
