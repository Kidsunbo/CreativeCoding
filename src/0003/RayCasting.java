package sample;

import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;
import javafx.stage.Stage;


import java.util.*;

class Wall{
    private static Random random = new Random(Calendar.getInstance().getTimeInMillis());
    private double x1;
    private double x2;
    private double y1;

    public double getX1() {
        return x1;
    }

    public double getX2() {
        return x2;
    }

    public double getY1() {
        return y1;
    }

    public double getY2() {
        return y2;
    }

    private double y2;

    public Wall(Scene scene){
        x1 = random.nextDouble()*scene.getWidth();
        x2 = random.nextDouble()*scene.getWidth();
        y1 = random.nextDouble()*scene.getHeight();
        y2 = random.nextDouble()*scene.getHeight();
    }

    public Wall(double x1,double y1,double x2,double y2){
        this.x1=x1;
        this.y1=y1;
        this.x2=x2;
        this.y2=y2;
    }

    public void draw(GraphicsContext gc){
        gc.setStroke(Color.WHITE);
        gc.moveTo(x1,y1);
        gc.lineTo(x2,y2);
        gc.stroke();
    }

}



public class RayCasting extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{
        Group root = new Group();
        Scene scene = new Scene(root,1280,960);



        Canvas canvas = new Canvas(scene.getWidth(),scene.getHeight());
        GraphicsContext gc = canvas.getGraphicsContext2D();
        gc.setFill(Color.BLACK);
        gc.fillRect(0,0,scene.getWidth(),scene.getHeight());

        root.getChildren().add(canvas);
        primaryStage.setScene(scene);

        //Create walls
        ArrayList<Wall> walls = new ArrayList<>();
        for(int i=0;i<5;i++){
            walls.add(new Wall(scene));
        }
        walls.add(new Wall(0,0,scene.getWidth(),0));
        walls.add(new Wall(scene.getWidth(),0,scene.getWidth(),scene.getHeight()));
        walls.add(new Wall(scene.getWidth(),scene.getHeight(),0,scene.getHeight()));
        walls.add(new Wall(0,scene.getHeight(),0,0));

        //Draw Walls
        for(Wall wall : walls){
            wall.draw(gc);
        }

        canvas.setOnMouseMoved(e->{
            gc.beginPath();
            gc.setFill(Color.BLACK);
            gc.clearRect(0,0,scene.getWidth(),scene.getHeight());
            gc.fillRect(0,0,scene.getWidth(),scene.getHeight());

            //Draw Walls
            for(Wall wall : walls){
                wall.draw(gc);
            }

            //Draw Light Point
            gc.setFill(Color.WHITE);
            gc.fillOval(e.getX()-5,e.getY()-5,10,10);

            //Draw light
            for(int i=0;i<360;i++){
                gc.beginPath();
                gc.moveTo(e.getX(),e.getY());
                double x1 = e.getX();
                double x2 = e.getX()+Math.sin(i/180.0*Math.PI);
                double y1 = e.getY();
                double y2 = e.getY()-Math.cos(i/180.0*Math.PI);
                double x = 0;
                double y = 0;
                double length = Double.MAX_VALUE;
                for(Wall wall:walls){
                    double x3 = wall.getX1();
                    double x4 = wall.getX2();
                    double y3 = wall.getY1();
                    double y4 = wall.getY2();
                    double den = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
                    if(den==0) continue;
                    double t = ((x1-x3)*(y3-y4)-(y1-y3)*(x3-x4))/den;
                    double u = -((x1-x2)*(y1-y3)-(y1-y2)*(x1-x3))/den;
                    if(u>=0&&u<=1.0&&t>0){
                        double p1 = x1+t*(x2-x1);
                        double p2 = y1+t*(y2-y1);
                        double r = Math.pow(p1-e.getX(),2)+Math.pow(p2-e.getY(),2);
                        if(length>r){
                            length=r;
                            x = p1;
                            y = p2;
                        }
                    }
                }
                gc.lineTo(x,y);
                gc.stroke();
                gc.closePath();

            }
        });
        primaryStage.setX(10);
        primaryStage.setY(10);
        primaryStage.show();

    }





    public static void main(String[] args) {
        launch(args);
    }
}
