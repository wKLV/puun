if(mouse & puun_CLICK) {
    if(!startMoving) {
    //    printf("%f %f %f\n", velX, velY, inertia);
        velX =0; velY=0;
        startMoving = true;
    }
    float dx = p2v(x) - lastPos.x ;
    float dy = p2v(y) - (-lastPos.y) ;

    if(!((dx>0) == (velX>0))||((dy>0) == (velY>0))) {
       // printf("Inversion %f %f\n", velX, velY);
        velX*= 0.75, velY *= 0.75;
    }
    velX*= 0.75, velY *= 0.75;
    velX += dx; velY-= dy;
    sprintf(text.text, "%f %f", velX, velY);
    inertia = velX*velX+velY*velY;
  //  if(inertia>0.05) finalX=velX, finalY=velY;
 //   else finalX =0, finalY=0;
    if(inertia<10.)inertia=10.;
    finalX=velX, finalY=velY;
    square_traslate(ball, p2v(x), -p2v(y));
}
else {
    startMoving = false;
    square_traslate(ball, lastPos.x+finalX/inertia, lastPos.y+finalY/inertia);
}

