# MT681
Library that simulates the behavior of an ISKRA MT681 smart meter data manipulation.

Example usgage:
```c
int main(void) {
    MT681 sml;
    sml.injectActivePower(0x382);
    sml.injectConsumption(0x3a26);

    for(int i; i < sml.getMessageLength(); i++) {
        printf("0x%02x, ", MT681::message[i]);
    }
}
```

It was designed be as portable as possible so it writes data with regard to endianness.
