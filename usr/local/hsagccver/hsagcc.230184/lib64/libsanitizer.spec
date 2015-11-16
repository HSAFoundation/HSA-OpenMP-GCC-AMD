# This spec file is read by gcc when linking.  It is used to specify the
# standard libraries we need in order to link with various sanitizer libs.

*link_libasan: -lrt -lrt -lpthread -ldl -lm

*link_libtsan: -lrt -lrt -lpthread -ldl -lm

*link_libubsan: -lrt -lpthread -ldl -lm

*link_liblsan: -lrt -lpthread -ldl -lm

