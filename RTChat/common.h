
typedef struct {
    char delta;
} Delta;

enum Errors {
    Success,
    SocketCreation,
    InvalidAddress,
    InvalidPort,
    ConnectionFaled,
    Bind,
    Listen,
    Accept,
};

typedef struct {
    enum Errors error;
    const char* message;
} ErrorMessage;