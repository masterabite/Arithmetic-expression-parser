/*
  функция поддерживает операции + и -, также учитывает круглые скобки.
  Функция является частично рекурсивной, вызовы применяются только при
  встречи открывающихся скобок, тем самым вычисляя выражение между
  соответствующими скобками
*/

int calc(string& str, int l, int r) {
        int res = 0, temp = 0, sign = 1;
        for (int i = l; i < r; ++i) {
            if (str[i] >= '0' && str[i] <= '9') {
                temp *= 10;
                temp += str[i] - '0';
            } else                 
            if (str[i] == '+') {
                res += temp*sign;
                sign = 1;
                temp = 0;
            } else 
            if (str[i] == '-') {
                res += temp*sign;
                sign = -1;
                temp = 0;
            } else
            if (str[i] == '(') {
                int b_l = i+1, b_r;
                int b_cnt = 1;

                for (b_r = b_l; b_r < r; ++b_r) {
                    if (str[b_r] == '(') {
                        ++b_cnt;
                    } else 
                    if (str[b_r] == ')') {
                        if (--b_cnt == 0) break;
                    }
                }

                temp = calc(str, b_l, b_r);
                i = b_r;
            }
        }
        
        res += temp*sign;
        return res;
    }
