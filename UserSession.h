#ifndef USERSESSION_H
#define USERSESSION_H

class UserSession {
public:
    static void setUserId(int id) { userId = id; }
    static int getUserId() { return userId; }

private:
    static int userId;
};

#endif // USERSESSION_H
