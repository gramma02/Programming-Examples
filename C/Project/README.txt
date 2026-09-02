Student registry in C, Spring-Summer 2021

Keeps students (AEM, first/last name, failed courses) in a dynamic array of pointers. The table grows and shrinks by factor K. You can add, remove, update failed courses, sort by AEM, and search (linear, or binary after sort).

On top of that, each student has a sorted course list, and there is a hash table on last names (separate chaining). The hash table resizes by load factor. Extra commands: register/unregister courses, list courses, and look up by last name.
