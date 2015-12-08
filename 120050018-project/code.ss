#lang racket/gui
(require racket/mpair)
(require racket/vector)
(define (make-2d-vector r c initial)
  (build-vector r (lambda (x) (make-vector c initial))))

(define (2d-vector-ref vec r c)
  (vector-ref (vector-ref vec r) c))

(define (2d-vector-set! vec r c val)
  (let ((v (vector-ref vec r)))
    (begin
      (vector-set! v c val)
      (vector-set! vec r v))))
(define-syntax while
  (syntax-rules ( : )
    [ (while bexp : statements)
      (begin
        (define(loop)
          (cond[bexp statements
                     (loop)]))
        (loop))]))
(define board (make-2d-vector 8 8 0))

(define (initial-board) (let* ([i 0]) (while (< i 8) : 
                                             ( begin (let* ([j 0])  (while (< j 8) :      
                                                                           (begin (cond [(>= i 5)
                                                                                         (cond [(= (remainder (+ i j) 2) 1)  (2d-vector-set! board i j 2)])]
                                                                                        [(<= i 2) (cond [(= (remainder (+ i j) 2) 1) (2d-vector-set! board i j 1)])]
                                                                                        [else (2d-vector-set! board i j 0)])
                                                                                  (set! j (+ j 1))))) (set! i (+ i 1))))))
(initial-board)

(define (inside-board x y)
  (and (>= x 0) (< x 8) (>= y 0) (< y 8))) 
(define (moves n)
  (cond [(equal? n 1) (list (list 1 1) (list -1 1)  (list 2 2) (list -2 2))]
        [(equal? n 2) (list (list 1 -1) (list -1 -1) (list 2 -2) (list -2 -2))]
        [(or (equal? n (mcons 10 #t)) 
             (equal? n (mcons 10 #f))
             (equal? n (mcons 20 #t))
             (equal? n (mcons 20 #f)))
         (list (list 1 1) (list -1 1) (list 1 -1) 
               (list -1 -1)(list 2 2) (list -2 2) (list 2 -2) (list -2 -2))]))
(define (opposite n)
  (cond [(or (equal? n 1) (equal? n (mcons 10 #t))) '(2 (mcons 20 #t) (mcons 20 #f) )]
        [(or (equal? n 2) (equal? n (mcons 20 #t))) '(1 (mcons 10 #t) (mcons 10 #f))]
        [else '()])
  )
(define (member? x list)
  (if (null? list) #f
      (if (equal? (car list) x) #t
          (member? x (cdr list)))))
(define (singleton x) (list x))
(define (only-jumph list1)
  (if (null? list1) '()
      (if (= (abs (caar list1)) 2) (singleton (car list1))
          (only-jumph (cdr list1)))))

(define (only-jump list)
  ( if (null? (only-jumph list)) list 
       (only-jumph list)))

(define (valid-moves x y board)
  (define (vacant x y)
    (equal? (2d-vector-ref board y x) 0))
  (define (preferable x y)
    (if  (inside-board x y)
         (vacant x y)
         #f))
  
   
      (cond [(inside-board x y) (let* ([n (2d-vector-ref board y x )])
                              (define (enemypresent x y) (member? (2d-vector-ref board y x ) (opposite n)))
                              ( if (equal? n 0) '()
                       (let* ([a (filter (lambda (t) (preferable (+ x (car t)) (+ y (cadr t)))) (moves n))])
                          (filter (lambda (t) (if (= (abs (car t)) 2)
                                                 (enemypresent (+ x (/ (car t) 2)) (+ y (/ (cadr t) 2)))
                                                           #t))
                                           a ))))]))

(define (admissible turn)
  (if (equal? turn "CPU") (list 1 (mcons 10 #t) (mcons 10 #f))
      (list 2 (mcons 20 #t) (mcons 20 #f))))
(define (valid-movesh x y cboard turn)
  (let* ([q (scanforjump cboard turn)]
         [w (car q)]
         [e (cdr q)]
         [k (2d-vector-ref cboard y x )])
  (cond [(member? k (admissible turn))
          (if w (if (equal? (list x y) e) (only-jumph (valid-moves x y cboard)) '())
        (valid-moves x y cboard))]
        [(member? k (admissible (oppos turn))) '()]
        [else '()])))

(define (scanforjump cboard turn)
 (begin (define check #f)
        
        (define accum '())
  (let* ([j 0]) (while (< j 8) : 
                    (begin   (begin  (let* ([i 0])
                                 (while (< i 8) : 
                                            (begin   
                                            (let* ([a (2d-vector-ref cboard j i)])
                                                 (cond [(member? a (admissible turn))
                      (cond [(not (null? (only-jumph (valid-moves i j cboard)))) (begin 
                                                   (set! check #t)
                                                                (set! accum (list i j)))])]))            
                                                   (set! i (+ i 1))))))
                             (set! j (+ j 1)))))
                               
  (cons check accum)))
                                 
  
  
(define dummy #t)

(define (check-king-set cboard)
  (let* ([j 0]) (while (< j 8) : 
                       (begin  (let* ([i 0])
                                 (while (< i 8) : (begin (let* ([a (2d-vector-ref cboard j i)])
                                                           (cond [(equal? a (- 2 (/ j 7)))
                                                                  (2d-vector-set! cboard j i (mcons (* 10 a) #f))])
                                                           (set! i (+ i 1))       ))))
                               (set! j (+ j 7))))))
(define (restrainjump cboard)
  (let* ([j 0]) (while (< j  8) : 
                       (begin  (let* ([i 0])
                                 (while (< i 8) : (begin (let* ([a (2d-vector-ref cboard j i)])
                                                           (cond [(mpair? a) 
                                                                  (cond [(not (mcdr a))   (2d-vector-set! cboard j i (mcons (mcar a) #t))])])
                                                           (set! i (+ i 1))       )))
                                 ) (set! j (+ j 1))))))

(define board2 (make-2d-vector 8 8 0))

  (define (copy cboard)
    (define leverage (make-2d-vector 8 8 0))
   (begin  (let* ([j 0]) (while (< j  8) : 
                       (begin  (let* ([i 0])
                                 (while (< i 8) : (begin (let* ([a (2d-vector-ref cboard j i)])
                                   (2d-vector-set!  leverage j i a))
                                                         (set! i (+ i 1)))))      
                                  (set! j (+ j 1))))) leverage))

(define (transform cboard x y turn)
  (let* ([l (valid-movesh x y cboard turn)]
         )
    
    (if (null? l) '()  (map (lambda (z) 
                                        (let (
                                              [a (2d-vector-ref cboard y x) ])
                                          (begin 
                                          (define board1 (copy cboard))
                                           
                                            
                                           (if (= (abs (car z)) 2)
                                                (begin
                                                  (2d-vector-set! board1 (+ y (/ (cadr z) 2)) (+ x (/ (car z) 2)) 0)
                                                   (2d-vector-set! board1 (+ y (cadr z)) (+ x (car z))  a)
                                                   (2d-vector-set! board1 y x 0)
                                                   (restrainjump board1)
                                                   (check-king-set board1)
                                                   )
                                                (begin 
                                                   (2d-vector-set! board1  (+ y (cadr z)) (+ x (car z))  a)
                                                   (2d-vector-set! board1  y x 0)
                                                   (restrainjump board1)
                                                   (check-king-set board1)
                                                   ))     
                                            board1))) l)))) 



(define (evaluatestatic whoplays cboard)
  (define accum '())
(begin  (let* ([n (signfactor whoplays)]
    [a (mcons (* 10 n) #t)]
    [b (mcons (* 10 n) #f)]
    [c (list n a b)])
     (let* ([j 0]) (while (< j  8) : 
                       (begin  (let* ([i 0])
                                 (while (< i 8) : (begin (let* ([a1 (2d-vector-ref cboard j i)])
                                                    (cond [(member? a1 c)
                                                    (set! accum  (append accum (transform cboard i j whoplays)))]))    
                                                           (set! i (+ i 1))))       
                                 ) (set! j (+ j 1))))))
        accum))

 
(define (sum vector)
  (let* ((n (vector-count (lambda (x) (not (= x -999))) vector))
         (i 0)
         (sum 0))
    (while (< i n) : (begin (set! sum (+ sum (vector-ref vector i)))
                            (set! i (+ i 1))))
    sum)
  )

(define (count y board)
  (sum (vector-map (lambda (x) (vector-count (lambda (z) (if (and (mpair? z) (mpair? y))
                                                             (equal? (mcar y) (mcar z))
                                                             (equal? y z) )) x)) board )))
(define (signfactor Whoplays)
  (cond [(equal? Whoplays "Human")
         2 ]
        [(equal? Whoplays "CPU") 1]))
(define (oppos Whoplays)
  (if (equal? Whoplays "Human") "CPU"
      "Human"))

(define (countsideways board)
(begin  (define a 0)
  (define b 0)
  (define c 0)
  (define d 0)
 (let* ([x 1]
        [y 2]
    [i (mcons (* 10 x) #t)]
    [j (mcons (* 10 x) #f)]
    [k (list  i j)]
    
     [p (mcons (* 10 y) #t)]
    [q (mcons (* 10 y) #f)]
    [r (list  p q)]) (let* ([j 0]) (while (< j  8) : 
                       (begin  (let* ([i 0])
                                 (while (< i 8) : (begin (let* ([a1 (2d-vector-ref board j i)])
                                                (cond [(not (equal? a1 0))   (cond [(equal? a1 x) (set! a (+ a 1))]
                                                           [(member? a1 k) (set! b (+ b 1))]
                                                         [ (equal? a1 y) (set! c (+ c 1))]
                                                           [(member? a1 r) (set! d (+ d 1))]  
                                                          )]))
                                                             (set! i (+ i 7))))       
                                 ) (set! j (+ j 1)))))))
  (list a b c d))
  
(define (cost a b c d)
  (+ (+ (+ a (* b 6)) (* c 3)) (* d 8)))
(define (evaluateboard cboard Whoplays)
  (let* ([a (signfactor Whoplays)]
         [b (signfactor (oppos Whoplays))]
         [q (count a cboard)]
         [w (count b cboard)]
         [e (count (or (mcons (* 10 a) #t) (mcons (* 10 a) #f)) cboard)]
         [r (count (or (mcons (* 10 b) #t) (mcons (* 10 b) #f)) cboard)]
         [l (countsideways cboard)]
         [z (car l)]
         [x (cadr l)]
         [c (caddr l)]
         [v (cadddr l)])
  (begin   (define cost1 0)
    (define cost2 0)
    (cond [(equal? Whoplays "CPU") (begin (set! cost1 (cost (- q z) (- e x) z x))
                                          (set! cost2 (cost (- w c) (- r v) c v)))]
          [(equal? Whoplays "Human") (begin (set! cost1 (cost (- q c) (- e v) c v))
                                          (set! cost2 (cost (- w z) (- r x) z x)))])
    (- cost1 cost2))))
    


(define (find x list)
 (define (findh x list n)
  (if (null? list) #f
     (if (= (car list) x) (cons (car list) n)
      (findh x (cdr list) (+ n 1)))))
  (findh x list 0))
(define (findmax f list)
  (let* ([v (map (lambda (x) (f x)) list)]
    [maxim (apply max v)]
    [a (find maxim v)]
    [b (cdr a)]
    )
    (list-ref list b)))

(define (gameended board turn)
  (if (null? (evaluateboard board turn))
      #t #f))
(define (win board turn)
  (if (gameended board turn)
  (if (equal? turn "CPU") "Human" "CPU")
  #f))
  

  

    (define (minimax alpha beta board depth turn)
     
   (begin  
     
     (define dummy #t)
     (define a alpha)
      (define b beta)
      
          
      (define (gothrough boardlist)
             (if  (null? boardlist) a
                (let* ([x (car boardlist)]
                [y (minimax a b x (- depth 1) (oppos turn))])
                   (begin     (set! a (max a y))
                  (if (>= a b)  b
                  (gothrough (cdr boardlist)))))))
      (define (gothrough1 boardlist)
             (if  (null? boardlist) b
                (let* ([x (car boardlist)]
                [y (minimax a b x (- depth 1) (oppos turn))])
                   (begin     (set! b (min b y))
                  (if (>= a b)  a
                  (gothrough1 (cdr boardlist)))))))
    (if (or (= depth 0) (gameended board turn)) 
          (evaluateboard board "CPU")
          (if (equal? turn "CPU")
            
            (gothrough (evaluatestatic turn board))
           
            (gothrough1 (evaluatestatic turn board))))))
           
(define depth 6)
    (define (make-move board)
      (define a -9999)
      (define b 9999)
   (findmax (lambda (x) (minimax a b x (- depth 1) "Human")) (evaluatestatic "CPU" board))) 
              
              
    
    
  (define frame (new frame% [label "checkers"][width 500][height 500]))
 


              
  
    ; Derive a new canvas (a drawing window) class to handle events
  (define my-canvas%
    (class canvas%
      (define/override (on-event event)
        (and (send msg set-label (string-append (number->string (send event get-x))"," (number->string (send event get-y))))
             (send test reader (list (number->string (send event get-x)) (number->string (send event get-y)) (send event get-event-type )))))
             
      (define/override (on-char event)
        (define pressed (send event get-key-code))
        (if (char? pressed) (and (send msg set-label (string-append  "key pressed:" (make-string 1 pressed)))
                                 (send test reader (list (make-string 1 pressed)))) '()))
     
      (super-new)))
   
  (define tester%
    (class object%
      (super-new)
      (define command '("0" "0" 'enter))
      (init-field [p1 1])
      (init-field [p2 1])
      (init-field [p3 1])
      (init-field [p4 1])
      (define/public (reader x)
        (begin (set! command x)
        (cond ((equal? (caddr x) 'right-down)
            (begin
              (set! p1  (quotient (string->number (car x)) 50))
            (set! p2  (quotient (string->number (cadr x)) 50))))
       ((equal? (caddr x) 'left-down)
               (begin
             (set! p3  (quotient (string->number (car x)) 50))
            (set! p4  (quotient (string->number (cadr x)) 50)))))))
       
      (define/public (outer)
        command)
      (define/public (position-finder)
    (list (quotient (string->number (car command)) 50) 
                      (quotient (string->number (car (cdr command))) 50)))
             
      (define/public (lup)
        (list p1 p2))
      (define/public (ldown)
        (list p3 p4))
              
      
      
             
            
      ))
  (define test (new tester% ))
  
  
  (define mycanvas (new my-canvas% [parent frame]
                        [paint-callback (lambda (canvas dc)
                                    (paint dc))]))
  
 
  (define msg (new message% [parent frame]
                            [label "No events so far..."]))
  
 (define path (new dc-path%))
   (define (paint dc) (send dc draw-bitmap face-bitmap 0 0))

 
  (define face-bitmap (make-object bitmap% 450 450))
  (define bm-dc (make-object bitmap-dc% face-bitmap))
  (send bm-dc clear)
  
  (define black-pen (make-object pen% "BLACK" 1 'solid))
  (define no-brush (make-object brush% "BLACK" 'transparent))
  (define blue-brush (make-object brush% "BLUE" 'solid))
  (define yellow-brush (make-object brush% "YELLOW" 'solid))
  (define red-pen (make-object pen% "RED" 2 'solid))
  (define red-brush (make-object brush% "RED" 'solid))
  (define green-brush (make-object brush% "GREEN" 'solid))
  (define orange-brush (make-object brush% "ORANGE" 'solid))
  (define black-brush (make-object brush% "BLACK" 'solid))
 (define olive-brush (make-object brush% "OLIVE" 'solid))
 (define violetred-brush (make-object brush% "VIOLETRED" 'solid))
  
 (define (draw-box dc x y n)
   (cond [(= n 2)
    (begin
    (send dc set-brush blue-brush)
    (send dc draw-rectangle x y 50 50)
    (send dc set-brush yellow-brush)
    (send dc draw-rectangle x (+ y 50) 50 50)
    (send dc set-brush yellow-brush)
    (send dc draw-rectangle (+ x 50) y 50 50)
    (send dc set-brush blue-brush)
    (send dc draw-rectangle (+ x 50) (+ y 50) 50 50)
    )]
         [(= n 4)
          (begin
            (draw-box dc x y 2)
            (draw-box dc (+ 100 x) (+ 100 y) 2)
            (draw-box dc (+ 100 x) y 2)
            (draw-box dc x (+ 100 y) 2))]
         [(= n 8)
          (begin 
            (draw-box dc x y 4)
            (draw-box dc (+ 200 x) (+ 200 y) 4)
            (draw-box dc (+ 200 x) y 4)
            (draw-box dc x (+ 200 y) 4)
            )]))
            

 (define (draw-goalpos dc x y length board3 )
   (let* ([i 0]) (while (< i 8) : 
                                             ( begin (let* ([j 0])  (while (< j 8) :      
                 (begin (let* ([x1 (+ x (* length j))]
                          [y1 (+ (* length i) y)]
                          [a (2d-vector-ref board3 i j)] )
                          
                         (cond ((equal? a 0)
                                (send dc set-brush no-brush))
                               ((equal? a 1)
                                (send dc set-brush green-brush)
                               (send dc draw-ellipse x1  y1 length length))
                               ((equal? a 2)
                                (send dc set-brush red-brush)
                              (send dc draw-ellipse x1  y1 length length))
                               ((equal? a 5)
                                 (send dc set-brush orange-brush)
                               (send dc draw-ellipse x1  y1 length length))
                               
                                     [(or (equal? (mcons 10 #t) a) (equal? a (mcons 10 #f)))
                                            (send dc set-brush olive-brush)
                                            (send dc draw-ellipse x1  y1 length length)]
                                          [(or (equal?  a (mcons 20 #t)) (equal? a (mcons 20 #f))) 
                                            (send dc set-brush violetred-brush)
                                            (send dc draw-ellipse x1  y1 length length)]))
                                          (set! j (+ j 1))))) (set! i (+ i 1))))))

 (define green-pen (new pen% [color "green"] [width 2])) 
  
  
 

  (send frame show #t)
 (define (possible x y cboard turn)
   (define (possibleh x y v) 
     (if (null? v) (set! dummy #t)
    (begin (2d-vector-set! cboard   (+ y (car (cdr (car v)))) (+ x (car (car v))) 5 )
           (possibleh x y (cdr v)))))
   (possibleh x y (valid-movesh x y cboard turn)))
      
(define (possiblem x y cboard turn)
  (begin
    (possible x y cboard turn)
  cboard))
 

(define (playboardh x y x1 y1 v cboard) 
       (if (null? v) cboard
         (if (member? (list (- x1 x) (- y1 y)) v)
       (begin 
           (2d-vector-set! cboard  y1  x1 (2d-vector-ref cboard y x ))
           (cond [(= (abs (- x1 x)) 2) (2d-vector-set! cboard (/ (+ y y1) 2) (/ (+ x x1) 2) 0)])  
           (2d-vector-set! cboard  y  x 0 )
           (restrainjump cboard)
           (check-king-set cboard)
           cboard)
      
      cboard )))
   
 
         

   
   
   
   
   
   

(define graph%
      (class object%
      (super-new)
      (define/public (graphics board7)
             (send bm-dc clear)
             (draw-box bm-dc 0 0 8)
             (draw-goalpos bm-dc 0 0 50 board7)
             (send mycanvas refresh)
             (sleep/yield 0.2))))
  
(define x 0)
(define y 0)
(define x1 0)
(define y1 0)
  (define graph (new graph%))
                  
  
  (define (mover cboard turn)
   (begin (define aa1 (copy cboard))
    
 (cond [ (equal?  (caddr (send test outer)) 'left-down)
         
         (let* ((new (possiblem  (car (send test position-finder))
                                 (cadr (send test position-finder)) aa1 turn)))
            (send graph graphics new)
          (mover cboard turn))]
        [ (equal?  (caddr (send test outer)) 'right-down)
          (let* ((new1 (playboardh (car (send test ldown)) (cadr (send test ldown))
                                  (car (send test lup)) (cadr (send test lup))
                                  (valid-movesh 
                                   (car (send test ldown)) (cadr (send test ldown))
                                   cboard turn) cboard))
                 (new2 (make-move new1)))
             (send graph graphics new1)
            
            
             (mover new2 turn))]    
            (else (send graph graphics cboard)
                     (mover cboard turn)))))
  
 (mover board "Human")
  
  
  
  