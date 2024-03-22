import hashlib
from itertools import product

def generate_md5_combinations(list1, list2):
    md5_hashes = []

    for l1 in list1:
        for l2 in list2:
            word = l1+l2
            md5_hash = hashlib.md5(word.encode('utf-8')).hexdigest()
            print( word +":  "+ md5_hash)
        # Add the hash to the list

    # Generate all combinations of words from the two lists
    return

# Example usage
list1 = ['Sam', 'Ruby', 'Arizona', 'Jetta', 'Focus', 'Mmmbop', 'Cashier', 'Tacos', 'Purple', 'Spot', 'Jackastors', 'Winter', 'Pretzels', 'Modesto', 'Sharon', 'Grace', 'Gault', 'Facts', 'Sgaulsoft' , 'Winter', 'Android']
list2 = ['2009', '2023', '0315', '0723','0707', '0309', '03', '09', '07', '23', '15', '26', '27', '97', '3', '9', '200', '07072023', '070723', '03152009', '15032009', '031509', '150309', '1997']

generate_md5_combinations(list1, list2)
