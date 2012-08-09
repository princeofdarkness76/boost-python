#!/usr/bin/python
print "===========================\n============================="
import jahnfuchs_ext

u = jahnfuchs_ext.U()
print "   vvvvvvvvvvvvvvvvvvvvv make_T vvvvvvvvvvvvvvvvvvvvvvvvvvvvv"
t = jahnfuchs_ext.make_T()
print "   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
#t.name = "myTect"
#u.set_shared_ptr(t)
#u.print_shared_ptr()

#t_2 = u.get_shared_ptr()
#t_2.name = "change_name"
#u.print_shared_ptr()

print "---\n---\n---"
u.set_weak_ptr(t)
print(u.name_via_weak_ptr())




