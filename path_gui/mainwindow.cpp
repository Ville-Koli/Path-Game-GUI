#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "gameboard.hh"
#include "constants.hh"
#include <QPushButton>
#include <QTimer>
#include <iostream>
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // MainWindow constuctor.
    ui->setupUi(this);

    generate_board();
    timer_ = new QTimer(this);
    play_timer_ = new QTimer(this);

    connect(ui->reset_button, &QPushButton::clicked, this, &MainWindow::reset_board);
    connect(timer_, &QTimer::timeout, this, &MainWindow::update_timer);
    connect(play_timer_, &QTimer::timeout, this, &MainWindow::play);
    connect(ui->play_moves, &QPushButton::clicked, this, &MainWindow::start_play);
    connect(ui->pause, &QPushButton::clicked, this, &MainWindow::pause);
}

MainWindow::~MainWindow()
{
    // MainWindow deconstructor.
    delete ui;
    delete game_board_;
    for(auto i : ui_game_board_){
        delete i;
    }
}

void MainWindow::play(){
    /* Method moves pieces automatically
     * Method takes no parameters
     * and does not return a value.
     */
    if(move_!= completed_game_.size()){
        update_all_labels(completed_game_.at(move_), completed_game_.at(move_+1));
        update_board();
        move_ += 2;
    }else{
        play_timer_->stop();
        timer_->stop();
    }
}

void MainWindow::update_all_labels(Ui_point *start, Ui_point *end)
{
    /* Method is for updating labels on
     * the userinterace.
     * Method takes Ui_point* start and Ui_point* end
     * as its parameters
     * Method does not return a value.
     */
    update_is_move_legal(start, end);
    ui->moves_spent_label->setText(QString::fromStdString(EMPTY_MOVES + std::to_string(moves_spent_)));
    ui->points_label->setText(QString::fromStdString(EMPTY_POINTS + std::to_string(player_points_)));
}

void MainWindow::update_all_labels(Point start, Point end)
{
    /* Method is for updating labels on
     * the userinterace.
     * Method takes Point start and Point end
     * as its parameters
     * Method does not return a value.
     */
    update_is_move_legal(start, end);
    ui->moves_spent_label->setText(QString::fromStdString(EMPTY_MOVES + std::to_string(moves_spent_)));
    ui->points_label->setText(QString::fromStdString(EMPTY_POINTS + std::to_string(player_points_)));
}

void MainWindow::reset_all_ui_texts()
{
    /* Method is for resetting labels on
     * the userinterace.
     * Method takes no parameters
     * Method does not return a value.
     */
    ui->show_time->display(0);
    ui->is_move_legal->setText(QString::fromStdString(EMPTY_LEGAL_MOVE));
    ui->moves_spent_label->setText(QString::fromStdString(EMPTY_MOVES));
    ui->points_label->setText(QString::fromStdString(EMPTY_POINTS));
    ui->is_paused->setText(QString::fromStdString(FALSE_PAUSE_TEXT));
    ui->is_won->setText(QString::fromStdString(HAS_NOT_WON));
}

void MainWindow::pause()
{
    /* Method is for pausing the board
     * (stopping timer and disabling player ability to
     * move pieces on the board)
     * Method takes no parameters
     * Method does not return a value.
     */
    if(not is_board_frozen_){
        timer_->stop();
        pause_board(true);
    }else{
        timer_->start(DELAY);
        pause_board(false);
    }
    play_timer_->stop();
}

void MainWindow::pause_board(bool set_value)
{
    /* Method is for pausing the board (disabling
     * the button on ui). Method also updates
     * is_board_frozen_ to be current state of
     * the board.
     * Method takes bool set_value as its parameters
     * Method does not return a value.
     */
    is_board_frozen_ = set_value;

    if(set_value){
        ui->is_paused->setText(QString::fromStdString(TRUE_PAUSE_TEXT));
    }else{
        ui->is_paused->setText(QString::fromStdString(FALSE_PAUSE_TEXT));
    }

    for(Ui_point* point : ui_game_board_){
        point->button->setDisabled(set_value);
    }
}

void MainWindow::start_play(){
    /* Method is starts autoplay.
     * Method takes no parameters.
     * Method does not return a value.
     */
    reset_board();
    pause_board(true);
    play_timer_->start(DELAY);
    timer_->start(DELAY);
}


void MainWindow::reset_board(){
    /* Method is for resetting the board
     * Method takes no parameters
     * Method does not return a value.
     */
    game_board_ = new GameBoard();
    update_board();
    timer_->stop();
    play_timer_->stop();
    time_s_ = 0;
    player_points_ = 0;
    moves_spent_ = 0;
    move_ = 0;
    reset_all_ui_texts();
    pause_board(false);
}

void MainWindow::update_timer(){
    /* Method is for updating timer display
     * Method takes no parameters.
     * Method does not return a value.
     */
    ui->show_time->display(time_s_);
    time_s_ += 1;
}

void MainWindow::set_button_color(QPushButton* button, const Point& current_point){
    /* Method is for setting button color
     * Method takes a QPushButton* button and Point current_point
     * as its parameters
     * Method does not return a value.
     */
    for(const Ui_color_pair& pair : COLORS){
        if (game_board_->which_slot({current_point.x, current_point.y}) == pair.color){
            button->setStyleSheet(QString::fromStdString(pair.css));
            break;
        }
    }
}

void MainWindow::have_you_won()
{
    /* Method is for checking whether player has won and
     * updating the is_won label whether it's true or false.
     * Method takes no parameters
     * Method does not return a value.
     */
    if(game_board_->is_game_over()){
        ui->is_won->setText(QString::fromStdString(HAS_WON));
        timer_->stop();
        play_timer_->stop();
        pause_board(true);
    }
}

void MainWindow::update_board(){
    /* Method is for updating the ui board.
     * Method takes no parameters.
     * Method does not return a value.
     */
    for(Ui_point* current_point : ui_game_board_){
        set_button_color(current_point->button, current_point->button_coords);
    }
}

void MainWindow::update_is_move_legal(Ui_point* start, Ui_point* destination)
{
    /* Method is for updating is_move_legal label on
     * the userinterace.
     * Method takes Ui_point* start and Ui_point* destination
     * as its parameters
     * Method does not return a value.
     */
    if(game_board_->move({start->button_coords.x, start->button_coords.y}, {destination->button_coords.x, destination->button_coords.y})){
        ui->is_move_legal->setText(QString::fromStdString(LEGAL_MOVE));
        moves_spent_ += 1;

        have_you_won();

        if(moves_spent_ < 63){
            player_points_ = floor(31*sin((M_PI*moves_spent_)/62));
        }

    }else{
        ui->is_move_legal->setText(QString::fromStdString(NOT_LEGAL_MOVE));
    }
}

void MainWindow::update_is_move_legal(Point start, Point end)
{
    /* Method is for updating is_move_legal label on
     * the userinterace.
     * Method takes Point start and Point end
     * as its parameters
     * Method does not return a value.
     */
    if(game_board_->move(start, end)){
        ui->is_move_legal->setText(QString::fromStdString(LEGAL_MOVE));
        moves_spent_ += 1;

        have_you_won();

        if(moves_spent_ < 63){
            player_points_ = floor(31*sin((M_PI*moves_spent_)/62));
        }

    }else{
        ui->is_move_legal->setText(QString::fromStdString(NOT_LEGAL_MOVE));
    }
}

void MainWindow::select(){
    /* Method is for moving pieces on the board.
     * Method takes no parameters.
     * Method does not return a value.
     */
    if(last_selected_ == nullptr){
        for(Ui_point* current_point : ui_game_board_){
            if(current_point->button == sender()
                    && (game_board_->which_slot({current_point->button_coords.x, current_point->button_coords.y}) == GREEN
                    || game_board_->which_slot({current_point->button_coords.x, current_point->button_coords.y}) == RED)){
                last_selected_ = current_point;
                break;
            }
        }
    }else{
        Ui_point* destination = ui_game_board_.at(0);
        for(Ui_point* current_point : ui_game_board_){
            if(current_point->button == sender()){
                destination = current_point;
                break;
            }
        }

        update_all_labels(last_selected_, destination);
        update_board();

        if(!timer_->isActive()){timer_->start(1000);}

        last_selected_ = nullptr;
    }
}


void MainWindow::generate_board(const Point generate_to, int width, int height){
    /* Method is for generating the buttons for the
     * ui_game_board_.
     * Method takes Point generate_to, int width, int height
     * as its parameters.
     * Method does not return a value.
     */
    for(int y = 0; y < static_cast<int>(ROWS); ++y){
        for(int x = 0; x < static_cast<int>(COLUMS); ++x){
            QPushButton* new_button = new QPushButton(this);
            new_button->setGeometry(generate_to.x + x*width, generate_to.y + y*height, width, height);
            connect(new_button, &QPushButton::clicked, this, &MainWindow::select);
            Ui_point* current_ui_point = new Ui_point({new_button, {x, y}});
            set_button_color(new_button, {x, y});
            ui_game_board_.push_back(current_ui_point);

        }
    }
}
