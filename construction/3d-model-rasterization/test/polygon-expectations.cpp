
const char *test_1_step_0 = R"delimiter(
Polygon 0 containing 4 segments:
  0: (x: -3.000, y: -3.000)-(x: -3.000, y: 3.000)
  1: (x: -3.000, y: 3.000)-(x: 3.000, y: 3.000)
  2: (x: 3.000, y: 3.000)-(x: 3.000, y: -3.000)
  3: (x: 3.000, y: -3.000)-(x: -3.000, y: -3.000)
)delimiter";

const char *test_1_step_1 = R"delimiter(
Polygon 0 containing 4 segments:
  0: (x: -3.000, y: 0.000)-(x: -3.000, y: -3.000)
  1: (x: -3.000, y: -3.000)-(x: 3.000, y: -3.000)
  2: (x: 3.000, y: -3.000)-(x: 3.000, y: 0.000)
  3: (x: 3.000, y: 0.000)-(x: -3.000, y: 0.000)

Polygon 1 containing 4 segments:
  0: (x: 3.000, y: 0.000)-(x: 3.000, y: 3.000)
  1: (x: 3.000, y: 3.000)-(x: -3.000, y: 3.000)
  2: (x: -3.000, y: 3.000)-(x: -3.000, y: 0.000)
  3: (x: -3.000, y: 0.000)-(x: 3.000, y: 0.000)
)delimiter";

const char *test_1_step_2 = R"delimiter(
Polygon 0 containing 4 segments:
  0: (x: 0.000, y: -3.000)-(x: -3.000, y: -3.000)
  1: (x: -3.000, y: -3.000)-(x: -3.000, y: 0.000)
  2: (x: -3.000, y: 0.000)-(x: 0.000, y: 0.000)
  3: (x: 0.000, y: 0.000)-(x: 0.000, y: -3.000)

Polygon 1 containing 4 segments:
  0: (x: 0.000, y: 0.000)-(x: 3.000, y: 0.000)
  1: (x: 3.000, y: 0.000)-(x: 3.000, y: -3.000)
  2: (x: 3.000, y: -3.000)-(x: 0.000, y: -3.000)
  3: (x: 0.000, y: -3.000)-(x: 0.000, y: 0.000)

Polygon 2 containing 4 segments:
  0: (x: 0.000, y: 0.000)-(x: -3.000, y: 0.000)
  1: (x: -3.000, y: 0.000)-(x: -3.000, y: 3.000)
  2: (x: -3.000, y: 3.000)-(x: 0.000, y: 3.000)
  3: (x: 0.000, y: 3.000)-(x: 0.000, y: 0.000)

Polygon 3 containing 4 segments:
  0: (x: 0.000, y: 3.000)-(x: 3.000, y: 3.000)
  1: (x: 3.000, y: 3.000)-(x: 3.000, y: 0.000)
  2: (x: 3.000, y: 0.000)-(x: 0.000, y: 0.000)
  3: (x: 0.000, y: 0.000)-(x: 0.000, y: 3.000)
)delimiter";

const char *test_2_step_0 = R"delimiter(
Polygon 0 containing 4 segments:
  0: (x: -3.000, y: -3.000)-(x: -3.000, y: 3.000)
  1: (x: -3.000, y: 3.000)-(x: 3.000, y: 3.000)
  2: (x: 3.000, y: 3.000)-(x: 3.000, y: -3.000)
  3: (x: 3.000, y: -3.000)-(x: -3.000, y: -3.000)

Polygon 1 containing 4 segments:
  0: (x: -1.500, y: -1.500)-(x: -1.500, y: 1.500)
  1: (x: -1.500, y: 1.500)-(x: 1.500, y: 1.500)
  2: (x: 1.500, y: 1.500)-(x: 1.500, y: -1.500)
  3: (x: 1.500, y: -1.500)-(x: -1.500, y: -1.500)
)delimiter";

const char *test_2_step_1 = R"delimiter(
Polygon 0 containing 8 segments:
  0: (x: -3.000, y: 0.000)-(x: -3.000, y: -3.000)
  1: (x: -3.000, y: -3.000)-(x: 3.000, y: -3.000)
  2: (x: 3.000, y: -3.000)-(x: 3.000, y: 0.000)
  3: (x: 3.000, y: 0.000)-(x: 1.500, y: 0.000)
  4: (x: 1.500, y: 0.000)-(x: 1.500, y: -1.500)
  5: (x: 1.500, y: -1.500)-(x: -1.500, y: -1.500)
  6: (x: -1.500, y: -1.500)-(x: -1.500, y: 0.000)
  7: (x: -1.500, y: 0.000)-(x: -3.000, y: 0.000)

Polygon 1 containing 8 segments:
  0: (x: -1.500, y: 0.000)-(x: -1.500, y: 1.500)
  1: (x: -1.500, y: 1.500)-(x: 1.500, y: 1.500)
  2: (x: 1.500, y: 1.500)-(x: 1.500, y: 0.000)
  3: (x: 1.500, y: 0.000)-(x: 3.000, y: 0.000)
  4: (x: 3.000, y: 0.000)-(x: 3.000, y: 3.000)
  5: (x: 3.000, y: 3.000)-(x: -3.000, y: 3.000)
  6: (x: -3.000, y: 3.000)-(x: -3.000, y: 0.000)
  7: (x: -3.000, y: 0.000)-(x: -1.500, y: 0.000)
)delimiter";

const char *test_2_step_2 = R"delimiter(
Polygon 0 containing 6 segments:
  0: (x: 0.000, y: -3.000)-(x: -3.000, y: -3.000)
  1: (x: -3.000, y: -3.000)-(x: -3.000, y: 0.000)
  2: (x: -3.000, y: 0.000)-(x: -1.500, y: 0.000)
  3: (x: -1.500, y: 0.000)-(x: -1.500, y: -1.500)
  4: (x: -1.500, y: -1.500)-(x: 0.000, y: -1.500)
  5: (x: 0.000, y: -1.500)-(x: 0.000, y: -3.000)

Polygon 1 containing 6 segments:
  0: (x: 0.000, y: -1.500)-(x: 1.500, y: -1.500)
  1: (x: 1.500, y: -1.500)-(x: 1.500, y: 0.000)
  2: (x: 1.500, y: 0.000)-(x: 3.000, y: 0.000)
  3: (x: 3.000, y: 0.000)-(x: 3.000, y: -3.000)
  4: (x: 3.000, y: -3.000)-(x: 0.000, y: -3.000)
  5: (x: 0.000, y: -3.000)-(x: 0.000, y: -1.500)

Polygon 2 containing 6 segments:
  0: (x: 0.000, y: 1.500)-(x: -1.500, y: 1.500)
  1: (x: -1.500, y: 1.500)-(x: -1.500, y: 0.000)
  2: (x: -1.500, y: 0.000)-(x: -3.000, y: 0.000)
  3: (x: -3.000, y: 0.000)-(x: -3.000, y: 3.000)
  4: (x: -3.000, y: 3.000)-(x: 0.000, y: 3.000)
  5: (x: 0.000, y: 3.000)-(x: 0.000, y: 1.500)

Polygon 3 containing 6 segments:
  0: (x: 0.000, y: 3.000)-(x: 3.000, y: 3.000)
  1: (x: 3.000, y: 3.000)-(x: 3.000, y: 0.000)
  2: (x: 3.000, y: 0.000)-(x: 1.500, y: 0.000)
  3: (x: 1.500, y: 0.000)-(x: 1.500, y: 1.500)
  4: (x: 1.500, y: 1.500)-(x: 0.000, y: 1.500)
  5: (x: 0.000, y: 1.500)-(x: 0.000, y: 3.000)
)delimiter";

const char *test_3_step_0 = R"delimiter(
Polygon 0 containing 16 segments:
  0: (x: 68.000, y: 284.000)-(x: 109.000, y: 147.000)
  1: (x: 109.000, y: 147.000)-(x: 212.000, y: 66.000)
  2: (x: 212.000, y: 66.000)-(x: 355.000, y: 101.000)
  3: (x: 355.000, y: 101.000)-(x: 289.000, y: 255.000)
  4: (x: 289.000, y: 255.000)-(x: 257.000, y: 392.000)
  5: (x: 257.000, y: 392.000)-(x: 386.000, y: 335.000)
  6: (x: 386.000, y: 335.000)-(x: 443.000, y: 223.000)
  7: (x: 443.000, y: 223.000)-(x: 510.000, y: 125.000)
  8: (x: 510.000, y: 125.000)-(x: 625.000, y: 32.000)
  9: (x: 625.000, y: 32.000)-(x: 800.000, y: 74.000)
  10: (x: 800.000, y: 74.000)-(x: 820.000, y: 248.000)
  11: (x: 820.000, y: 248.000)-(x: 686.000, y: 372.000)
  12: (x: 686.000, y: 372.000)-(x: 470.000, y: 494.000)
  13: (x: 470.000, y: 494.000)-(x: 316.000, y: 556.000)
  14: (x: 316.000, y: 556.000)-(x: 118.000, y: 448.000)
  15: (x: 118.000, y: 448.000)-(x: 68.000, y: 284.000)

Polygon 1 containing 5 segments:
  0: (x: 553.000, y: 262.000)-(x: 654.000, y: 200.000)
  1: (x: 654.000, y: 200.000)-(x: 683.000, y: 287.000)
  2: (x: 683.000, y: 287.000)-(x: 572.000, y: 380.000)
  3: (x: 572.000, y: 380.000)-(x: 502.000, y: 373.000)
  4: (x: 502.000, y: 373.000)-(x: 553.000, y: 262.000)
)delimiter";

const char *test_3_step_1 = R"delimiter(
Polygon 0 containing 4 segments:
  0: (x: 305.000, y: 550.000)-(x: 213.333, y: 500.000)
  1: (x: 213.333, y: 500.000)-(x: 455.097, y: 500.000)
  2: (x: 455.097, y: 500.000)-(x: 330.903, y: 550.000)
  3: (x: 330.903, y: 550.000)-(x: 305.000, y: 550.000)

Polygon 1 containing 3 segments:
  0: (x: 330.903, y: 550.000)-(x: 316.000, y: 556.000)
  1: (x: 316.000, y: 556.000)-(x: 305.000, y: 550.000)
  2: (x: 305.000, y: 550.000)-(x: 330.903, y: 550.000)

Polygon 2 containing 5 segments:
  0: (x: 213.333, y: 500.000)-(x: 121.667, y: 450.000)
  1: (x: 121.667, y: 450.000)-(x: 547.902, y: 450.000)
  2: (x: 547.902, y: 450.000)-(x: 470.000, y: 494.000)
  3: (x: 470.000, y: 494.000)-(x: 455.097, y: 500.000)
  4: (x: 455.097, y: 500.000)-(x: 213.333, y: 500.000)

Polygon 3 containing 5 segments:
  0: (x: 121.667, y: 450.000)-(x: 118.000, y: 448.000)
  1: (x: 118.000, y: 448.000)-(x: 103.366, y: 400.000)
  2: (x: 103.366, y: 400.000)-(x: 636.426, y: 400.000)
  3: (x: 636.426, y: 400.000)-(x: 547.902, y: 450.000)
  4: (x: 547.902, y: 450.000)-(x: 121.667, y: 450.000)

Polygon 4 containing 12 segments:
  0: (x: 103.366, y: 400.000)-(x: 88.122, y: 350.000)
  1: (x: 88.122, y: 350.000)-(x: 266.810, y: 350.000)
  2: (x: 266.810, y: 350.000)-(x: 257.000, y: 392.000)
  3: (x: 257.000, y: 392.000)-(x: 352.053, y: 350.000)
  4: (x: 352.053, y: 350.000)-(x: 512.568, y: 350.000)
  5: (x: 512.568, y: 350.000)-(x: 502.000, y: 373.000)
  6: (x: 502.000, y: 373.000)-(x: 572.000, y: 380.000)
  7: (x: 572.000, y: 380.000)-(x: 607.806, y: 350.000)
  8: (x: 607.806, y: 350.000)-(x: 709.774, y: 350.000)
  9: (x: 709.774, y: 350.000)-(x: 686.000, y: 372.000)
  10: (x: 686.000, y: 372.000)-(x: 636.426, y: 400.000)
  11: (x: 636.426, y: 400.000)-(x: 103.366, y: 400.000)

Polygon 5 containing 4 segments:
  0: (x: 88.122, y: 350.000)-(x: 72.878, y: 300.000)
  1: (x: 72.878, y: 300.000)-(x: 278.489, y: 300.000)
  2: (x: 278.489, y: 300.000)-(x: 266.810, y: 350.000)
  3: (x: 266.810, y: 350.000)-(x: 88.122, y: 350.000)

Polygon 6 containing 5 segments:
  0: (x: 352.053, y: 350.000)-(x: 386.000, y: 335.000)
  1: (x: 386.000, y: 335.000)-(x: 403.813, y: 300.000)
  2: (x: 403.813, y: 300.000)-(x: 535.541, y: 300.000)
  3: (x: 535.541, y: 300.000)-(x: 512.568, y: 350.000)
  4: (x: 512.568, y: 350.000)-(x: 352.053, y: 350.000)

Polygon 7 containing 4 segments:
  0: (x: 607.806, y: 350.000)-(x: 667.484, y: 300.000)
  1: (x: 667.484, y: 300.000)-(x: 763.806, y: 300.000)
  2: (x: 763.806, y: 300.000)-(x: 709.774, y: 350.000)
  3: (x: 709.774, y: 350.000)-(x: 607.806, y: 350.000)

Polygon 8 containing 6 segments:
  0: (x: 72.878, y: 300.000)-(x: 68.000, y: 284.000)
  1: (x: 68.000, y: 284.000)-(x: 78.175, y: 250.000)
  2: (x: 78.175, y: 250.000)-(x: 291.143, y: 250.000)
  3: (x: 291.143, y: 250.000)-(x: 289.000, y: 255.000)
  4: (x: 289.000, y: 255.000)-(x: 278.489, y: 300.000)
  5: (x: 278.489, y: 300.000)-(x: 72.878, y: 300.000)

Polygon 9 containing 5 segments:
  0: (x: 403.813, y: 300.000)-(x: 429.259, y: 250.000)
  1: (x: 429.259, y: 250.000)-(x: 572.548, y: 250.000)
  2: (x: 572.548, y: 250.000)-(x: 553.000, y: 262.000)
  3: (x: 553.000, y: 262.000)-(x: 535.541, y: 300.000)
  4: (x: 535.541, y: 300.000)-(x: 403.813, y: 300.000)

Polygon 10 containing 5 segments:
  0: (x: 667.484, y: 300.000)-(x: 683.000, y: 287.000)
  1: (x: 683.000, y: 287.000)-(x: 670.667, y: 250.000)
  2: (x: 670.667, y: 250.000)-(x: 817.839, y: 250.000)
  3: (x: 817.839, y: 250.000)-(x: 763.806, y: 300.000)
  4: (x: 763.806, y: 300.000)-(x: 667.484, y: 300.000)

Polygon 11 containing 4 segments:
  0: (x: 78.175, y: 250.000)-(x: 93.139, y: 200.000)
  1: (x: 93.139, y: 200.000)-(x: 312.571, y: 200.000)
  2: (x: 312.571, y: 200.000)-(x: 291.143, y: 250.000)
  3: (x: 291.143, y: 250.000)-(x: 78.175, y: 250.000)

Polygon 12 containing 9 segments:
  0: (x: 429.259, y: 250.000)-(x: 443.000, y: 223.000)
  1: (x: 443.000, y: 223.000)-(x: 458.724, y: 200.000)
  2: (x: 458.724, y: 200.000)-(x: 814.483, y: 200.000)
  3: (x: 814.483, y: 200.000)-(x: 820.000, y: 248.000)
  4: (x: 820.000, y: 248.000)-(x: 817.839, y: 250.000)
  5: (x: 817.839, y: 250.000)-(x: 670.667, y: 250.000)
  6: (x: 670.667, y: 250.000)-(x: 654.000, y: 200.000)
  7: (x: 654.000, y: 200.000)-(x: 572.548, y: 250.000)
  8: (x: 572.548, y: 250.000)-(x: 429.259, y: 250.000)

Polygon 13 containing 4 segments:
  0: (x: 93.139, y: 200.000)-(x: 108.102, y: 150.000)
  1: (x: 108.102, y: 150.000)-(x: 334.000, y: 150.000)
  2: (x: 334.000, y: 150.000)-(x: 312.571, y: 200.000)
  3: (x: 312.571, y: 200.000)-(x: 93.139, y: 200.000)

Polygon 14 containing 4 segments:
  0: (x: 458.724, y: 200.000)-(x: 492.908, y: 150.000)
  1: (x: 492.908, y: 150.000)-(x: 808.736, y: 150.000)
  2: (x: 808.736, y: 150.000)-(x: 814.483, y: 200.000)
  3: (x: 814.483, y: 200.000)-(x: 458.724, y: 200.000)

Polygon 15 containing 6 segments:
  0: (x: 108.102, y: 150.000)-(x: 109.000, y: 147.000)
  1: (x: 109.000, y: 147.000)-(x: 168.765, y: 100.000)
  2: (x: 168.765, y: 100.000)-(x: 350.914, y: 100.000)
  3: (x: 350.914, y: 100.000)-(x: 355.000, y: 101.000)
  4: (x: 355.000, y: 101.000)-(x: 334.000, y: 150.000)
  5: (x: 334.000, y: 150.000)-(x: 108.102, y: 150.000)

Polygon 16 containing 5 segments:
  0: (x: 492.908, y: 150.000)-(x: 510.000, y: 125.000)
  1: (x: 510.000, y: 125.000)-(x: 540.914, y: 100.000)
  2: (x: 540.914, y: 100.000)-(x: 802.989, y: 100.000)
  3: (x: 802.989, y: 100.000)-(x: 808.736, y: 150.000)
  4: (x: 808.736, y: 150.000)-(x: 492.908, y: 150.000)

Polygon 17 containing 3 segments:
  0: (x: 168.765, y: 100.000)-(x: 212.000, y: 66.000)
  1: (x: 212.000, y: 66.000)-(x: 350.914, y: 100.000)
  2: (x: 350.914, y: 100.000)-(x: 168.765, y: 100.000)

Polygon 18 containing 5 segments:
  0: (x: 540.914, y: 100.000)-(x: 602.742, y: 50.000)
  1: (x: 602.742, y: 50.000)-(x: 700.000, y: 50.000)
  2: (x: 700.000, y: 50.000)-(x: 800.000, y: 74.000)
  3: (x: 800.000, y: 74.000)-(x: 802.989, y: 100.000)
  4: (x: 802.989, y: 100.000)-(x: 540.914, y: 100.000)

Polygon 19 containing 3 segments:
  0: (x: 602.742, y: 50.000)-(x: 625.000, y: 32.000)
  1: (x: 625.000, y: 32.000)-(x: 700.000, y: 50.000)
  2: (x: 700.000, y: 50.000)-(x: 602.742, y: 50.000)
)delimiter";

const char *test_3_step_2 = R"delimiter(
Polygon 0 containing 3 segments

Polygon 1 containing 4 segments

Polygon 2 containing 4 segments

Polygon 3 containing 4 segments

Polygon 4 containing 4 segments

Polygon 5 containing 4 segments

Polygon 6 containing 4 segments

Polygon 7 containing 5 segments

Polygon 8 containing 5 segments

Polygon 9 containing 3 segments

Polygon 10 containing 4 segments

Polygon 11 containing 4 segments

Polygon 12 containing 4 segments

Polygon 13 containing 4 segments

Polygon 14 containing 4 segments

Polygon 15 containing 5 segments

Polygon 16 containing 3 segments

Polygon 17 containing 4 segments

Polygon 18 containing 4 segments

Polygon 19 containing 4 segments

Polygon 20 containing 6 segments

Polygon 21 containing 5 segments

Polygon 22 containing 5 segments

Polygon 23 containing 5 segments

Polygon 24 containing 3 segments

Polygon 25 containing 4 segments

Polygon 26 containing 3 segments

Polygon 27 containing 5 segments

Polygon 28 containing 4 segments

Polygon 29 containing 4 segments

Polygon 30 containing 4 segments

Polygon 31 containing 3 segments

Polygon 32 containing 6 segments

Polygon 33 containing 4 segments

Polygon 34 containing 4 segments

Polygon 35 containing 4 segments

Polygon 36 containing 5 segments

Polygon 37 containing 5 segments

Polygon 38 containing 4 segments

Polygon 39 containing 3 segments

Polygon 40 containing 3 segments

Polygon 41 containing 6 segments

Polygon 42 containing 4 segments

Polygon 43 containing 4 segments

Polygon 44 containing 5 segments

Polygon 45 containing 4 segments

Polygon 46 containing 6 segments

Polygon 47 containing 5 segments

Polygon 48 containing 3 segments

Polygon 49 containing 4 segments

Polygon 50 containing 5 segments

Polygon 51 containing 4 segments

Polygon 52 containing 4 segments

Polygon 53 containing 4 segments

Polygon 54 containing 4 segments

Polygon 55 containing 4 segments

Polygon 56 containing 6 segments

Polygon 57 containing 3 segments

Polygon 58 containing 4 segments

Polygon 59 containing 4 segments

Polygon 60 containing 5 segments

Polygon 61 containing 4 segments

Polygon 62 containing 4 segments

Polygon 63 containing 4 segments

Polygon 64 containing 4 segments

Polygon 65 containing 3 segments

Polygon 66 containing 4 segments

Polygon 67 containing 5 segments

Polygon 68 containing 4 segments

Polygon 69 containing 4 segments

Polygon 70 containing 4 segments

Polygon 71 containing 4 segments

Polygon 72 containing 3 segments

Polygon 73 containing 5 segments

Polygon 74 containing 4 segments

Polygon 75 containing 4 segments

Polygon 76 containing 4 segments

Polygon 77 containing 4 segments

Polygon 78 containing 6 segments

Polygon 79 containing 4 segments

Polygon 80 containing 4 segments

Polygon 81 containing 4 segments

Polygon 82 containing 4 segments

Polygon 83 containing 5 segments

Polygon 84 containing 3 segments

Polygon 85 containing 6 segments

Polygon 86 containing 4 segments

Polygon 87 containing 4 segments

Polygon 88 containing 4 segments

Polygon 89 containing 5 segments

Polygon 90 containing 4 segments

Polygon 91 containing 4 segments

Polygon 92 containing 4 segments

Polygon 93 containing 4 segments

Polygon 94 containing 5 segments

Polygon 95 containing 4 segments

Polygon 96 containing 4 segments

Polygon 97 containing 4 segments

Polygon 98 containing 4 segments

Polygon 99 containing 5 segments

Polygon 100 containing 3 segments

Polygon 101 containing 3 segments

Polygon 102 containing 5 segments

Polygon 103 containing 4 segments

Polygon 104 containing 4 segments

Polygon 105 containing 4 segments

Polygon 106 containing 4 segments

Polygon 107 containing 4 segments

Polygon 108 containing 4 segments

Polygon 109 containing 4 segments

Polygon 110 containing 4 segments

Polygon 111 containing 5 segments

Polygon 112 containing 4 segments

Polygon 113 containing 4 segments

Polygon 114 containing 4 segments

Polygon 115 containing 5 segments

Polygon 116 containing 5 segments

Polygon 117 containing 3 segments

Polygon 118 containing 3 segments

Polygon 119 containing 4 segments

Polygon 120 containing 5 segments

Polygon 121 containing 4 segments

Polygon 122 containing 3 segments

Polygon 123 containing 3 segments
)delimiter";

const char *test_4_step_0 = R"delimiter(
Polygon 0 containing 6 segments:
  0: (x: -3.000, y: -3.000)-(x: -3.000, y: 0.000)
  1: (x: -3.000, y: 0.000)-(x: -3.000, y: 3.000)
  2: (x: -3.000, y: 3.000)-(x: 3.000, y: 3.000)
  3: (x: 3.000, y: 3.000)-(x: 3.000, y: 0.000)
  4: (x: 3.000, y: 0.000)-(x: 3.000, y: -3.000)
  5: (x: 3.000, y: -3.000)-(x: -3.000, y: -3.000)
)delimiter";

const char *test_4_step_1 = R"delimiter(
Polygon 0 containing 4 segments:
  0: (x: -3.000, y: 0.000)-(x: -3.000, y: -3.000)
  1: (x: -3.000, y: -3.000)-(x: 3.000, y: -3.000)
  2: (x: 3.000, y: -3.000)-(x: 3.000, y: 0.000)
  3: (x: 3.000, y: 0.000)-(x: -3.000, y: 0.000)

Polygon 1 containing 4 segments:
  0: (x: -3.000, y: 0.000)-(x: 3.000, y: 0.000)
  1: (x: 3.000, y: 0.000)-(x: 3.000, y: 3.000)
  2: (x: 3.000, y: 3.000)-(x: -3.000, y: 3.000)
  3: (x: -3.000, y: 3.000)-(x: -3.000, y: 0.000)
)delimiter";

const char *test_5_step_0 = R"delimiter(
Polygon 0 containing 8 segments:
  0: (x: 0.000, y: 100.000)-(x: 0.000, y: 200.000)
  1: (x: 0.000, y: 200.000)-(x: 100.000, y: 200.000)
  2: (x: 100.000, y: 200.000)-(x: 100.000, y: 100.000)
  3: (x: 100.000, y: 100.000)-(x: 175.000, y: 100.000)
  4: (x: 175.000, y: 100.000)-(x: 175.000, y: 0.000)
  5: (x: 175.000, y: 0.000)-(x: 75.000, y: 0.000)
  6: (x: 75.000, y: 0.000)-(x: 75.000, y: 100.000)
  7: (x: 75.000, y: 100.000)-(x: 0.000, y: 100.000)
)delimiter";

const char *test_5_step_1 = R"delimiter(
)delimiter";

const char *test_5_step_2 = R"delimiter(
)delimiter";

const char* test_6_step_0 = R"delimiter(
Polygon 0 containing 4 segments:
  0: (x: 83.000, y: 97.000)-(x: 132.000, y: 20.000)
  1: (x: 132.000, y: 20.000)-(x: 185.000, y: 94.000)
  2: (x: 185.000, y: 94.000)-(x: 133.000, y: 156.000)
  3: (x: 133.000, y: 156.000)-(x: 83.000, y: 97.000)

Polygon 1 containing 4 segments:
  0: (x: 42.000, y: 182.000)-(x: 68.000, y: 139.000)
  1: (x: 68.000, y: 139.000)-(x: 103.000, y: 181.000)
  2: (x: 103.000, y: 181.000)-(x: 75.000, y: 223.000)
  3: (x: 75.000, y: 223.000)-(x: 42.000, y: 182.000)

Polygon 2 containing 4 segments:
  0: (x: 164.000, y: 189.000)-(x: 191.000, y: 143.000)
  1: (x: 191.000, y: 143.000)-(x: 227.000, y: 187.000)
  2: (x: 227.000, y: 187.000)-(x: 199.000, y: 228.000)
  3: (x: 199.000, y: 228.000)-(x: 164.000, y: 189.000)
)delimiter";

const char* test_6_step_1 = R"delimiter(
Polygon 0 containing 3 segments:
  0: (x: 63.163, y: 147.000)-(x: 68.000, y: 139.000)
  1: (x: 68.000, y: 139.000)-(x: 74.667, y: 147.000)
  2: (x: 74.667, y: 147.000)-(x: 63.163, y: 147.000)

Polygon 1 containing 5 segments:
  0: (x: 74.667, y: 147.000)-(x: 103.000, y: 181.000)
  1: (x: 103.000, y: 181.000)-(x: 75.000, y: 223.000)
  2: (x: 75.000, y: 223.000)-(x: 42.000, y: 182.000)
  3: (x: 42.000, y: 182.000)-(x: 63.163, y: 147.000)
  4: (x: 63.163, y: 147.000)-(x: 74.667, y: 147.000)

Polygon 2 containing 5 segments:
  0: (x: 125.373, y: 147.000)-(x: 83.000, y: 97.000)
  1: (x: 83.000, y: 97.000)-(x: 132.000, y: 20.000)
  2: (x: 132.000, y: 20.000)-(x: 185.000, y: 94.000)
  3: (x: 185.000, y: 94.000)-(x: 140.548, y: 147.000)
  4: (x: 140.548, y: 147.000)-(x: 125.373, y: 147.000)

Polygon 3 containing 3 segments:
  0: (x: 140.548, y: 147.000)-(x: 133.000, y: 156.000)
  1: (x: 133.000, y: 156.000)-(x: 125.373, y: 147.000)
  2: (x: 125.373, y: 147.000)-(x: 140.548, y: 147.000)

Polygon 4 containing 3 segments:
  0: (x: 188.652, y: 147.000)-(x: 191.000, y: 143.000)
  1: (x: 191.000, y: 143.000)-(x: 194.273, y: 147.000)
  2: (x: 194.273, y: 147.000)-(x: 188.652, y: 147.000)

Polygon 5 containing 5 segments:
  0: (x: 194.273, y: 147.000)-(x: 227.000, y: 187.000)
  1: (x: 227.000, y: 187.000)-(x: 199.000, y: 228.000)
  2: (x: 199.000, y: 228.000)-(x: 164.000, y: 189.000)
  3: (x: 164.000, y: 189.000)-(x: 188.652, y: 147.000)
  4: (x: 188.652, y: 147.000)-(x: 194.273, y: 147.000)
)delimiter";

const char* test_7_step_0 = R"delimiter(
Polygon 0 containing 6 segments:
  0: (x: 118.000, y: 172.000)-(x: 74.000, y: 114.000)
  1: (x: 74.000, y: 114.000)-(x: 132.000, y: 20.000)
  2: (x: 132.000, y: 20.000)-(x: 194.000, y: 108.000)
  3: (x: 194.000, y: 108.000)-(x: 149.000, y: 172.000)
  4: (x: 149.000, y: 172.000)-(x: 133.000, y: 156.000)
  5: (x: 133.000, y: 156.000)-(x: 118.000, y: 172.000)

Polygon 1 containing 4 segments:
  0: (x: 42.000, y: 182.000)-(x: 68.000, y: 139.000)
  1: (x: 68.000, y: 139.000)-(x: 103.000, y: 181.000)
  2: (x: 103.000, y: 181.000)-(x: 75.000, y: 223.000)
  3: (x: 75.000, y: 223.000)-(x: 42.000, y: 182.000)

Polygon 2 containing 4 segments:
  0: (x: 164.000, y: 189.000)-(x: 191.000, y: 143.000)
  1: (x: 191.000, y: 143.000)-(x: 227.000, y: 187.000)
  2: (x: 227.000, y: 187.000)-(x: 199.000, y: 228.000)
  3: (x: 199.000, y: 228.000)-(x: 164.000, y: 189.000)
)delimiter";

const char* test_7_step_1 = R"delimiter(
Polygon 0 containing 3 segments:
  0: (x: 53.488, y: 163.000)-(x: 68.000, y: 139.000)
  1: (x: 68.000, y: 139.000)-(x: 88.000, y: 163.000)
  2: (x: 88.000, y: 163.000)-(x: 53.488, y: 163.000)

Polygon 1 containing 5 segments:
  0: (x: 88.000, y: 163.000)-(x: 103.000, y: 181.000)
  1: (x: 103.000, y: 181.000)-(x: 75.000, y: 223.000)
  2: (x: 75.000, y: 223.000)-(x: 42.000, y: 182.000)
  3: (x: 42.000, y: 182.000)-(x: 53.488, y: 163.000)
  4: (x: 53.488, y: 163.000)-(x: 88.000, y: 163.000)

Polygon 2 containing 8 segments:
  0: (x: 111.172, y: 163.000)-(x: 74.000, y: 114.000)
  1: (x: 74.000, y: 114.000)-(x: 132.000, y: 20.000)
  2: (x: 132.000, y: 20.000)-(x: 194.000, y: 108.000)
  3: (x: 194.000, y: 108.000)-(x: 155.328, y: 163.000)
  4: (x: 155.328, y: 163.000)-(x: 140.000, y: 163.000)
  5: (x: 140.000, y: 163.000)-(x: 133.000, y: 156.000)
  6: (x: 133.000, y: 156.000)-(x: 126.438, y: 163.000)
  7: (x: 126.438, y: 163.000)-(x: 111.172, y: 163.000)

Polygon 3 containing 3 segments:
  0: (x: 126.438, y: 163.000)-(x: 118.000, y: 172.000)
  1: (x: 118.000, y: 172.000)-(x: 111.172, y: 163.000)
  2: (x: 111.172, y: 163.000)-(x: 126.438, y: 163.000)

Polygon 4 containing 3 segments:
  0: (x: 155.328, y: 163.000)-(x: 149.000, y: 172.000)
  1: (x: 149.000, y: 172.000)-(x: 140.000, y: 163.000)
  2: (x: 140.000, y: 163.000)-(x: 155.328, y: 163.000)

Polygon 5 containing 3 segments:
  0: (x: 179.261, y: 163.000)-(x: 191.000, y: 143.000)
  1: (x: 191.000, y: 143.000)-(x: 207.364, y: 163.000)
  2: (x: 207.364, y: 163.000)-(x: 179.261, y: 163.000)

Polygon 6 containing 5 segments:
  0: (x: 207.364, y: 163.000)-(x: 227.000, y: 187.000)
  1: (x: 227.000, y: 187.000)-(x: 199.000, y: 228.000)
  2: (x: 199.000, y: 228.000)-(x: 164.000, y: 189.000)
  3: (x: 164.000, y: 189.000)-(x: 179.261, y: 163.000)
  4: (x: 179.261, y: 163.000)-(x: 207.364, y: 163.000)
)delimiter";

const char* union_test_1 = R"delimiter(
Polygon 0 containing 8 segments:
  0: (x: 86.000, y: 207.000)-(x: 144.000, y: 99.000)
  1: (x: 144.000, y: 99.000)-(x: 225.940, y: 144.067)
  2: (x: 225.940, y: 144.067)-(x: 271.000, y: 94.000)
  3: (x: 271.000, y: 94.000)-(x: 368.000, y: 161.000)
  4: (x: 368.000, y: 161.000)-(x: 282.000, y: 241.000)
  5: (x: 282.000, y: 241.000)-(x: 219.587, y: 190.619)
  6: (x: 219.587, y: 190.619)-(x: 182.000, y: 247.000)
  7: (x: 182.000, y: 247.000)-(x: 86.000, y: 207.000)
)delimiter";
