using Raylib_cs;
using genie.cast;

namespace genie.services.raylib {

    public class RaylibScreenService {
        
        private int fps;
        private Dictionary<string, Texture2D> textureCache;
        private Dictionary<(bool, bool, bool, bool), dynamic> circleSectorsDict;

        /***************************************************************
        * Constructor:
        *   - Initialize window
        *   - Set target FPS
        *   - Initialize the circle sectors dictionary
        *
        * Input:
        *   - windowSize: int 2-tuple, example: (600, 800)
        *   - title: string, the name of the window of the game. Default value is "Genie Game"
                    But you can change this to something else when you create the object
                    (example: "Asteroid")
        *   - fps: int, what frame rate do you want to use? Default is 60.
        ****************************************************************/
        public RaylibScreenService((int, int) windowSize, string title = "Genie Game", int fps = 60) {
            // 
            Raylib.InitWindow(windowSize.Item1, windowSize.Item2, title);

            this.SetFPS(fps);

            this.textureCache = new Dictionary<string, Texture2D>();

            this.circleSectorsDict = new Dictionary<(bool, bool, bool, bool), dynamic>();
            this.circleSectorsDict.Add((false, false, false, false), (0, 360));
            this.circleSectorsDict.Add((false, false, false, true), (0, 90));
            this.circleSectorsDict.Add((false, false, true, false), (0, -90));
            this.circleSectorsDict.Add((false, false, true, true), (-90, 90));
            this.circleSectorsDict.Add((false, true, false, false), (-90, -180));
            this.circleSectorsDict.Add((false, true, false, true), ((-90, -180), (0, 90)));
            this.circleSectorsDict.Add((false, true, true, false), (0, -180));
            this.circleSectorsDict.Add((false, true, true, true), (90, -180));

            this.circleSectorsDict.Add((true, false, false, false), (90, 180));
            this.circleSectorsDict.Add((true, false, false, true), (0, 180));
            this.circleSectorsDict.Add((true, false, true, false), ((90, 180), (0, -90)));
            this.circleSectorsDict.Add((true, false, true, true), (-90, 180));
            this.circleSectorsDict.Add((true, true, false, false), (90, 270));
            this.circleSectorsDict.Add((true, true, false, true), (0, 270));
            this.circleSectorsDict.Add((true, true, true, false), (90, 360));
            this.circleSectorsDict.Add((true, true, true, true), (0, 360));
        }

        /***************************************************************
        * Whatever needs to be done to initialize / re-initialize Screen Service
        ****************************************************************/
        public void Initialize() {
            // Nothing for now
        }

        /***************************************************************
        * (Private) Load Texture:
        * Return a texture given an Actor
        ****************************************************************/
        private Texture2D LoadTexture(Actor actor) {
            string imagePath = actor.GetPath();
            
            Image image_f = Raylib.LoadImage(imagePath);
            Raylib.ImageFlipHorizontal(ref image_f);
            
            Texture2D texture = Raylib.LoadTexture(imagePath);
            Texture2D texture_f = Raylib.LoadTextureFromImage(image_f);
            
            // When saving a texture, save both the normal image
            // and the flipped image
            if (!this.textureCache.Keys.Contains(imagePath)) {
                this.textureCache.Add(imagePath, texture);
                this.textureCache.Add(imagePath + "_f", texture_f);
            }

            return actor.GetFlipped() ? texture_f : texture;
        }

        /***************************************************************
        * Load textures
        ****************************************************************/
        public void LoadTexturesToCache(List<Actor> actors) {
            foreach (Actor actor in actors) {
                this.LoadTexture(actor);
            }
        }

        /***************************************************************
        * Set the fps to the declared fps
        ****************************************************************/
        public void SetFPS(int fps = 60) {
            this.fps = fps;
            Raylib.SetTargetFPS(fps);
        }

        /***************************************************************
        * BeginDrawing()
        ****************************************************************/
        public void BeginDrawing()
        {
            Raylib.BeginDrawing();
        }

        /***************************************************************
        * BeginDrawing()
        ****************************************************************/
        public void FillScreen(Color color)
        {
            Raylib.ClearBackground(new Raylib_cs.Color(color.r, color.g, color.b, color.a));
        }

        /***************************************************************
        * UpdateScreen()
        ****************************************************************/
        public void UpdateScreen()
        {
            Raylib.EndDrawing();
        }

        /***************************************************************
        * CloseWindow()
        ****************************************************************/
        public void CloseWindow()
        {
            Raylib.CloseWindow();
        }

        /***************************************************************
        * Check to see if the X mark on the top right of the game window
        * is clicked
        ****************************************************************/
        public bool IsQuit() {
            return Raylib.WindowShouldClose();
        }

        /***************************************************************
        * Draw Text: Draw the input text (string)
        * Inputs:
        *       - text: The text you want to draw
        *       - font: The font you want to use (This does not work for now...)
        *       - font_size: default is 24
        *       - color: An RGB tuple. (0,0,0) is BLACK, and (255,255,255) is WHITE
        *               You can also pass a 4 entries tuple. the 4th entry determines opacity
        *       - position: A tuple in the form of (x, y)
        *       - antialias: Boolean. Default is True (This does not work for now)
        *       - position_center: A boolean that tells whether the position given should be
        *                           the center of the text image or the top-left corner.
        *               + True: treats the position as the center of the text image
        *               + False: treats the position as the top-left corner of the text image
        ****************************************************************/
        public void DrawText(string text, (float x, float y) position, string font = "", 
                            int fontSize = 24, Color? color = null,
                            bool antialias = true, bool positionCenter = false) {
            Raylib_cs.Color raylibColor = (color != null) ?
                    new Raylib_cs.Color(color.r, color.g, color.b, color.a) : Raylib_cs.Color.BLACK;

            if (positionCenter) {
                Image textImage = Raylib.ImageText(text,fontSize, raylibColor);
                Raylib.DrawText(text,
                                (int)(position.x - textImage.width/2),
                                (int)(position.y - textImage.height/2),
                                fontSize,
                                raylibColor);
            }
            else {
                Raylib.DrawText(text,
                                (int)(position.x),
                                (int)(position.y),
                                fontSize,
                                raylibColor);
            }
        }

        /***************************************************************
        * Draw Rectangle: Draw a rectangle
        * Inputs:
        *       - center: An (x, y) tuple indicating the center of the rectangle
        *       - width: the width of the rectangle
        *       - height: the height of the rectangle
        *       - color: An RGB tuple. (0,0,0) is BLACK, and (255,255,255) is WHITE
        *               You can also pass a 4 entries tuple. the 4th entry determines opacity
        *       - border_width: how many pixels you want the border to be
        *       
        *       - roundness: goes from 0 to 1:
        *                   0 = completely squared
        *                   1 = rounded like a circle
        ****************************************************************/
        public void DrawRectangle((float x, float y) center, float width, float height,
                                    Color? color = null, int borderWidth = 0, float roundness = 0) {
            float topleftX = center.x - width/2;
            float topleftY = center.y - height/2;
            Raylib_cs.Color raylibColor = (color != null) ?
                    new Raylib_cs.Color(color.r, color.g, color.b, color.a) : Raylib_cs.Color.BLACK;

            if (borderWidth <= 0) {
                Raylib.DrawRectangleRounded(new Rectangle(topleftX, topleftY, width, height), roundness, 60, raylibColor);
            }
            else {
                Raylib.DrawRectangleRoundedLines(new Rectangle(topleftX, topleftY, width, height), roundness, 60, borderWidth, raylibColor);
            }
        }

        /***************************************************************
        * Draw Rectangle: Draw a circle
        * Inputs:
        *       - center: An (x, y) tuple indicating the center of the circle
        *       - radius: well...
        *       - color: An RGB tuple. (0,0,0) is BLACK, and (255,255,255) is WHITE
        *               You can also pass a 4 entries tuple. the 4th entry determines opacity
        *       - width: How bold you want the border of the circle to be
        *               0: filled circle
        *               >0: empty circle with visible boundry line of 1px
        *       
        *       - draw_top_..., draw_bottom_...: Boolean. Use these parameters if want to draw
        *           only parts of the circle (top left, top right, bottom left, bottom right)
        ****************************************************************/
        public void DrawCircle((float x, float y) center, float radius, Color? color = null, int width = 0,
                                bool drawTopRight = false, bool drawTopLeft = false,
                                bool drawBottomLeft = false, bool drawBottomRight = false) {
            dynamic anglesData = this.circleSectorsDict[(drawTopRight, drawTopLeft, drawBottomLeft, drawBottomRight)];
            bool separatedQuads = anglesData is ((int, int), (int,int));
            Raylib_cs.Color raylibColor = (color != null) ?
                    new Raylib_cs.Color(color.r, color.g, color.b, color.a) : Raylib_cs.Color.BLACK;

            if (width == 0) {
                if (separatedQuads) {
                    Raylib.DrawCircleSector(new System.Numerics.Vector2(center.x, center.y), radius, anglesData.Item1.Item1, anglesData.Item1.Item2, 60, raylibColor);
                    Raylib.DrawCircleSector(new System.Numerics.Vector2(center.x, center.y), radius, anglesData.Item2.Item1, anglesData.Item2.Item2, 60, raylibColor);
                } else {
                    Raylib.DrawCircleSector(new System.Numerics.Vector2(center.x, center.y), radius, anglesData.Item1, anglesData.Item2, 60, raylibColor);
                }
            } else if (width > 0) {
                if (anglesData is (int,int) && anglesData.Item1 == 0 && anglesData.Item2 == 360) {
                    Rectangle equivRec = new Rectangle(center.x - radius, center.y - radius, 2*radius, 2*radius);
                    Raylib.DrawRectangleRoundedLines(equivRec, 1, 60, (int)width, raylibColor);
                } else if (separatedQuads) {
                    Raylib.DrawCircleSectorLines(new System.Numerics.Vector2(center.x, center.y), radius, anglesData.Item1.Item1, anglesData.Item1.Item2, 60, raylibColor);
                    Raylib.DrawCircleSectorLines(new System.Numerics.Vector2(center.x, center.y), radius, anglesData.Item2.Item1, anglesData.Item2.Item2, 60, raylibColor);
                } else {
                    Raylib.DrawCircleSectorLines(new System.Numerics.Vector2(center.x, center.y), radius, anglesData.Item1, anglesData.Item2, 60, raylibColor);
                }
            }
        }

        /***************************************************************
        * DrawActor():
        *       - Create the texture from the image whose path is stored in the actor
        *       - Use the dimensions and position of the actor to draw the texture to the screen
        ****************************************************************/
        public void DrawActor(Actor actor) {
            (float x, float y) center = (actor.GetX(), actor.GetY());
            string path = actor.GetPath();
            Texture2D texture = new Texture2D();
            try {
                if (this.textureCache.Keys.Contains(path)) {
                    texture = actor.GetFlipped() ? this.textureCache[path+"_f"] : this.textureCache[path];
                } else {
                    texture = LoadTexture(actor);
                }

                int frameWidth = actor.GetWidth();
                int frameHeight = actor.GetHeight();

                Raylib.DrawTexturePro(texture, new Rectangle(0,0,texture.width, texture.height),
                                    new Rectangle(center.x, center.y, frameWidth, frameHeight),
                                    new System.Numerics.Vector2(frameWidth/2, frameHeight/2),
                                    actor.GetRotation(), new Raylib_cs.Color(255,255,255,255));
                
                if (actor is AnimatedActor) {
                    ((AnimatedActor)actor).SetNextFrame();
                }
            }
            catch (Exception e) {
                Console.WriteLine(e.Message);
            }
        }

        /***************************************************************
        * DrawActors():
        *   Draw all the actors in the "actors" list in order:
        *        First thing in the list gets drawn first.
        *
        *   actors: actors that need to be drawn
        *   lerp: linear interpolation (don't worry about this)
        ****************************************************************/
        public void DrawActors(List<Actor> actors) {
            foreach (Actor actor in actors) {
                this.DrawActor(actor);
            }
        }

        /***************************************************************
        * This might be used in the future
        ****************************************************************/
        public void Release() {
            
        }
    }
}