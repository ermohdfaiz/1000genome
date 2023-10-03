stats "cent.txt" name "A"
set xrange[A_min_x-10:A_max_x+10]
set yrange[A_min_y-10:A_max_y+10]
set zrange[9:16]
set tics nomirror
set grid
set terminal gif animate delay 30 
#00size 1200,800
set output "3dplot.gif"
do for [i =0:int(A_blocks-1)] {splot "output_1.txt" index 0 w p pt 6 ps 2 lc rgb "blue" title "points",\
"output_1.txt" index 1 w p pt 6 ps 2 lc rgb "blue" title "points",\
"cent.txt" u 1:2:3 index i w p pt 7 ps 1 title "c1-".(i+1),\
"cent.txt" u 4:5:6 index i w p pt 7 ps 1 title "c2-".(i+1)}



