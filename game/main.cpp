#include <iostream>
#include <cstdlib>
#include <fstream>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <SDL.h>
using namespace std;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const string WINDOW_TITLE = "Game 2048";
SDL_Window* window ;
SDL_Renderer* renderer;
const char* number_bmp[12]= {"n0.bmp", "n2.bmp", "n4.bmp", "n8.bmp", "n16.bmp", "n32.bmp", "n64.bmp", "n128.bmp", "n256.bmp", "n512.bmp", "n1024.bmp", "n2048.bmp"};
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	//Khởi tạo là nullptr để tránh lỗi 'dangling pointer'
	SDL_Texture *texture = nullptr;
	//Nạp ảnh từ tên file (với đường dẫn)
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	//Nếu không có lỗi, chuyển đổi về dạng texture and và trả về
	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		//Đảm bảo việc chuyển đổi không có lỗi
		if (texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
void waitUntilKeyPressed();

//Ma trận bảng
int Matrix[4][4];
int score, highScore;// điểm

// Vẽ khung
void VeKhung()
{ SDL_Texture *n0 = loadTexture("n0.bmp", renderer);
        for(int i=0;i<=450;i+=150){
            for(int j=0;j<=450;j+=150){
                int x=i;
                int y=j;
                renderTexture(n0, renderer, x, y);
            }
        }
        SDL_RenderPresent(renderer);
}
// Tạo số
int TaoSo()
{
	int a=rand()%20;
	if (a == 0) return 4; // 5% hàm tạo số 4
	else return 2;		  //95% hàm tạo số 1
}
// Thêm số
bool themSo;
int ThemSo()
{
	int i, j, a;
	//Kiểm tra xem bảng còn chỗ trống không
	bool ConCho=false;
	for (int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			if (Matrix[i][j] == 0){
				ConCho=true;
				break;
			}
		}
	}
	//Tạo giá trị ngẫu nhiên
	if (ConCho){
		do{
			i = rand()%4;
			j = rand()%4;
		}while (Matrix[i][j]);
		Matrix[i][j] = TaoSo();
	}
}

void InBang()
{
	int i, j;
	VeKhung();
	for (i=0; i<4; i++){
		for (int j=0; j<4; j++){
            int x= 150*i;
            int y= 150*j;
             switch (Matrix[i][j]){
                case 0: {SDL_Texture *n0 = loadTexture("n0.bmp", renderer);
                        renderTexture(n0, renderer, x, y);
                        break;}
                case 2: {SDL_Texture *n2 = loadTexture("n2.bmp", renderer);
                        renderTexture(n2, renderer, x, y);
                        break;}
                case 4: {SDL_Texture *n4 = loadTexture("n4.bmp", renderer);
                        renderTexture(n4, renderer, x, y);
                        break;}
                case 8: {SDL_Texture *n8 = loadTexture("n8.bmp", renderer);
                        renderTexture(n8, renderer, x, y);
                        break;}
                case 16:{SDL_Texture *n16 = loadTexture("n16.bmp", renderer);
                        renderTexture(n16, renderer, x, y);
                        break;}
                case 32: {SDL_Texture *n32 = loadTexture("n32.bmp", renderer);
                        renderTexture(n32, renderer, x, y);
                        break;}
                case 64: {SDL_Texture *n64 = loadTexture("n64.bmp", renderer);
                        renderTexture(n64, renderer, x, y);
                        break;}
                case 128: {SDL_Texture *n128 = loadTexture("n128.bmp", renderer);
                        renderTexture(n128, renderer, x, y);
                        break;}
                case 256:{SDL_Texture *n256 = loadTexture("n256.bmp", renderer);
                        renderTexture(n256, renderer, x, y);
                        break;}
                case 512: {SDL_Texture *n512 = loadTexture("n512.bmp", renderer);
                        renderTexture(n512, renderer, x, y);
                        break;}
                case 1024: {SDL_Texture *n1024 = loadTexture("n1024.bmp", renderer);
                        renderTexture(n1024, renderer, x, y);
                        break;}
                case 2048: {SDL_Texture *n2048 = loadTexture("n2048.bmp", renderer);
                        renderTexture(n2048, renderer, x, y);
                        break;}
            }
            SDL_RenderPresent(renderer);
		}

	}
}

	//Điểm người chơi
	/*gotoxy(30, 1); cout << "High Score " << highScore;
	gotoxy(30, 3); cout << "Score : " << score;
	gotoxy(30, 6);
	cout << "=> Nhan ESC de thoat." */

bool ConOTrong();
bool ConOTrong()
{
	bool kq = false;
	for (int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			if (!Matrix[i][j]){
				kq = true;
			}
		}
	}
	return kq;
}
bool Thua()
{
	bool kq1, kq2;
	if (ConOTrong()) {kq1 = true;} //Còn ô trống thì vẫn tiếp tục chơi
	for (int i=0; i<3; i++){
		for (int j=0; j<3; j++){
			//Có 2 ô cạnh nhau cùng giá trị thì tiếp tục chơi
			if (Matrix[i][j]==Matrix[i][j+1] || Matrix[i][j]==Matrix[i+1][j]){
				kq2 = true;
			}
		}
	}
	if ((!kq1) && (!kq2)) return true;
	else return false;
}
bool Thang();
bool Thang(){
    int i, j;
    for (i=0; i<4; i++){
        for (j=0; j<4; j++){
            if (Matrix[i][j] == 2048)
                return true;
        }

    }
    return false;

}

void Down()
{
	int i, j, vt1, vt2, temp;
	for (i=0; i<4; i++){
		//Gop het nhung o trong ben duoi
		for (j=3; j>0; j--){
			if (!Matrix[i][j]){
				vt1=vt2=j;
				do{ vt2--;if (vt2==0) break;} while (!Matrix[i][vt2]);
				if (vt2 || (!vt2 && Matrix[i][vt2])){

					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[i][vt1];
					Matrix[i][vt1] = Matrix[i][vt2];
					Matrix[i][vt2]= temp;
					themSo = true;
				}
			}
		}
		//Cong 2 o co cung gia tri
		for (j=3; j>0; j--){
			if (Matrix[i][j]!=0 && Matrix[i][j]==Matrix[i][j-1]){
				Matrix[i][j]*=2;
				score+=Matrix[i][j];
				Matrix[i][j-1] = 0;
				themSo = true;
			}
		}
		//Gop het nhung o trong ben duoi
		for (j=3; j>0; j--){
			if (!Matrix[i][j]){
				vt1=vt2=j;
				do{ vt2--;if (vt2==0) break;} while (!Matrix[i][vt2]);
				if (vt2 || (!vt2 && Matrix[i][vt2])){

					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[i][vt1];
					Matrix[i][vt1] = Matrix[i][vt2];
					Matrix[i][vt2]= temp;
					themSo = true;
				}
			}
		}
	}
}

void Up()
{
	int i, j, vt1, vt2, temp;
	for (i=0; i<4; i++){
		//Gop het nhung o trong ben tren
		for (j=0; j<3; j++){
			if (!Matrix[i][j]){
				vt1=vt2=j;
				do{ vt2++;if (vt2==3) break;} while (!Matrix[i][vt2]);
				if (vt2!=3 || (vt2==3 && Matrix[i][vt2])){

					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[i][vt1];
					Matrix[i][vt1] = Matrix[i][vt2];
					Matrix[i][vt2]= temp;
					themSo = true;
				}
			}
		}
		//Cong 2 o co cung gia tri
		for (j=0; j<3; j++){
			if (Matrix[i][j]!=0 && Matrix[i][j]==Matrix[i][j+1]){
				Matrix[i][j]*=2;
				score+=Matrix[i][j];
				Matrix[i][j+1] = 0;
				themSo = true;
			}
		}
		//Gop het nhung o trong ben tren
		for (j=0; j<3; j++){
			if (!Matrix[i][j]){
				vt1=vt2=j;
				do{ vt2++;if (vt2==3) break;} while (!Matrix[i][vt2]);
				if (vt2!=3 || (vt2==3 && Matrix[i][vt2])){

					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[i][vt1];
					Matrix[i][vt1] = Matrix[i][vt2];
					Matrix[i][vt2]= temp;
					themSo = true;
				}
			}
		}
	}
}

void Right()
{
	int i, j, vt1, vt2, temp;
	for (j=0; j<4; j++){
		//Gop het nhung o trong ben phai
		for (i=3; i>0; i--){
			if (!Matrix[i][j]){
				vt1=vt2=i;
				do{ vt2--;if (vt2==0) break;} while (!Matrix[vt2][j]);
				if (vt2 || (!vt2 && Matrix[vt2][j])){

					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[vt1][j];
					Matrix[vt1][j] = Matrix[vt2][j];
					Matrix[vt2][j]= temp;
					themSo = true;
				}
			}
		}
		//Cong 2 o co cung gia tri
		for (i=3; i>0; i--){
			if (Matrix[i][j]!=0 && Matrix[i][j]==Matrix[i-1][j]){
				Matrix[i][j]*=2;
				score+=Matrix[i][j];
				Matrix[i-1][j] = 0;
				themSo = true;
			}
		}
		//Gop het nhung o trong ben phai
		for (i=3; i>0; i--){
			if (!Matrix[i][j]){
				vt1=vt2=i;
				do{ vt2--;if (vt2==0) break;} while (!Matrix[vt2][j]);
				if (vt2 || (!vt2 && Matrix[vt2][j])){

					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[vt1][j];
					Matrix[vt1][j] = Matrix[vt2][j];
					Matrix[vt2][j]= temp;
					themSo = true;
				}
			}
		}
	}
}

void Left()
{
	int i, j, vt1, vt2, temp;
	for (j=0; j<4; j++){
		//Gop het nhung o trong ben trai
		for (i=0; i<3; i++){
			if (!Matrix[i][j]){
				vt1=vt2=i;
				do{ vt2++;if (vt2==3) break;} while (!Matrix[vt2][j]);
				if (vt2!=3 || (vt2==3 && Matrix[vt2][j])){

					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[vt1][j];
					Matrix[vt1][j] = Matrix[vt2][j];
					Matrix[vt2][j]= temp;
					themSo = true;
				}
			}
		}
		//Cong 2 o co cung gia tri
		for (i=0; i<3; i++){
			if (Matrix[i][j]!=0 && Matrix[i][j]==Matrix[i+1][j]){
				Matrix[i][j]*=2;
				score+=Matrix[i][j];
				Matrix[i+1][j] = 0;
				themSo = true;
			}
		}
		//Gop het nhung o trong ben trai
		for (i=0; i<3; i++){
			if (!Matrix[i][j]){
				vt1=vt2=i;
				do{ vt2++;if (vt2==3) break;} while (!Matrix[vt2][j]);
				if (vt2!=3 || (vt2==3 && Matrix[vt2][j])){

					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[vt1][j];
					Matrix[vt1][j] = Matrix[vt2][j];
					Matrix[vt2][j]= temp;
					themSo = true;
				}
			}
		}
	}
}


void Play()
{
	int i, j;
    SDL_Event e;
    SDL_Texture *winn = loadTexture("win.bmp", renderer);
	//ifstream inScore("Score.in");
	//inScore >> highScore;
	//inScore.close();
	//Khởi tạo gái trị ban đầu bằng 0
	for (i=0; i<4; i++){
		for (j=0; j<4; j++){
			Matrix[i][j]=0;
		}
	}
	ThemSo();
	InBang();

	do{
		themSo = false;
        if ( SDL_WaitEvent(&e) == 0) continue;
        // Nếu có một phím được nhấn, thì xét phím đó là gì để xử lý tiếp
        if (e.type == SDL_KEYDOWN) {
            // Nếu nhấn phìm ESC thì thoát khỏi vòng lặp
            if (e.key.keysym.sym == SDLK_ESCAPE) break;
            if (e.key.keysym.sym == SDLK_LEFT) Left();
            if (e.key.keysym.sym == SDLK_RIGHT) Right();
            if (e.key.keysym.sym == SDLK_DOWN) Down();
            if (e.key.keysym.sym == SDLK_UP) Up();
            if (themSo) ThemSo();
            InBang();
            if (Thang()) {renderTexture(winn, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                    SDL_RenderPresent(renderer);
                     SDL_Delay(20000);
                     quitSDL(window, renderer);
                     }
            }

	}while (!Thua());
	SDL_Texture *lose = loadTexture("gameover.bmp", renderer);
                    renderTexture(lose, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                    SDL_RenderPresent(renderer);
                     SDL_Delay(2000);

}
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Surface* &windowSurface)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);


    windowSurface = SDL_GetWindowSurface(window);
    if (windowSurface == nullptr) logSDLError(std::cout, "CreateWindowSurface", true);

}
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
    //   SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);


    //Khi thông thường chạy với môi trường bình thường ở nhà
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
	//Thiết lập hình chữ nhật đích mà chúng ta muốn vẽ ảnh vào trong
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Truy vẫn texture để lấy chiều rộng và cao (vào chiều rộng và cao tương ứng của hình chữ nhật đích)
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    //Đưa toàn bộ ảnh trong texture vào hình chữ nhật đích
	SDL_RenderCopy(ren, tex, NULL, &dst);
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
	//Thiết lập hình chữ nhật đích mà chúng ta muốn vẽ ảnh vào trong
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
    dst.w = w;
    dst.h = h;
    //Đưa toàn bộ ảnh trong texture vào hình chữ nhật đích
    //(ảnh sẽ co dãn cho khớp với kích cỡ mới)
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

int main(int argc, char* argv[])
{
	srand(time(0));
	initSDL(window, renderer);
    SDL_Texture *start = loadTexture("start.bmp", renderer);
    SDL_Texture *intro = loadTexture("intro.bmp", renderer);
    SDL_Texture *background = loadTexture("background.bmp", renderer);
    SDL_Texture *winn = loadTexture("win.bmp", renderer);
    renderTexture(start, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    renderTexture(intro, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_RenderClear(renderer);
    renderTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(renderer);
		score =0;
		Play();
	return 0;
}

