int main() {
    int x = 10;
    int y = 20;
    int i = 0;
    
    if (x < y) {
        x = x + y;
    } else {
        y = y - x;
    }
    
    while (x > 0) {
        x = x - 1;
    }
    
    for (i = 0; i < 10; i = i + 1) {
        y = y + i;
    }
    
    return 0;
}

