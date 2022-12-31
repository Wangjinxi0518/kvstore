#ifndef __KV_COMMAND_H_
#define __KV_COMMAND_H_

#include <string>
#include <vector>

class Command {
  public:
   Command();
   ~Command();

  static void SetCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void GetCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void DeleteCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void FlushAllCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void ExitCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void ShutDownCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void ErrorCommandImpl(const std::vector<std::string>& argv, std::string* const reply);

  protected:

  private:
   static Command* command_;

   Command(Command& se);
   void operator =(const Command& se);
};

#endif // __KV_COMMAND_H_
