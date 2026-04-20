#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <string.h>

static const char *ONLINE_FALLBACK_URL = "https://htahrijouti-hash.github.io/mini-projet/";

static void normalize_to_file_url_path(char *path) {
  size_t i;
  for (i = 0; path[i] != '\0'; ++i) {
    if (path[i] == '\\') {
      path[i] = '/';
    }
  }
}

static int launch_browser_app_mode(const char *browser, const char *fileUrl) {
  char params[4096];
  HINSTANCE res;

  _snprintf(params, sizeof(params) - 1,
            "--app=\"%s\" --window-size=1200,800", fileUrl);
  params[sizeof(params) - 1] = '\0';

  res = ShellExecuteA(NULL, "open", browser, params, NULL, SW_SHOWNORMAL);
  return ((INT_PTR)res > 32);
}

static int open_url_default(const char *url) {
  return ((INT_PTR)ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL) > 32);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
  char exePath[MAX_PATH];
  char dirPath[MAX_PATH];
  char htmlPath[MAX_PATH];
  char fileUrl[4096];
  char *lastSlash;
  DWORD attr;

  (void)hInstance;
  (void)hPrevInstance;
  (void)pCmdLine;
  (void)nCmdShow;

  if (GetModuleFileNameA(NULL, exePath, MAX_PATH) == 0) {
    MessageBoxA(NULL, "Impossible de localiser l'application.", "Le Meilleur Quiz", MB_OK | MB_ICONERROR);
    return 1;
  }

  strncpy(dirPath, exePath, MAX_PATH - 1);
  dirPath[MAX_PATH - 1] = '\0';

  lastSlash = strrchr(dirPath, '\\');
  if (!lastSlash) {
    MessageBoxA(NULL, "Chemin invalide.", "Le Meilleur Quiz", MB_OK | MB_ICONERROR);
    return 1;
  }
  *lastSlash = '\0';

  _snprintf(htmlPath, MAX_PATH - 1, "%s\\index.html", dirPath);
  htmlPath[MAX_PATH - 1] = '\0';

  attr = GetFileAttributesA(htmlPath);
  if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY)) {
    if (launch_browser_app_mode("msedge.exe", ONLINE_FALLBACK_URL)) {
      return 0;
    }
    if (launch_browser_app_mode("chrome.exe", ONLINE_FALLBACK_URL)) {
      return 0;
    }
    if (open_url_default(ONLINE_FALLBACK_URL)) {
      return 0;
    }
    MessageBoxA(NULL, "index.html introuvable localement et ouverture web impossible.", "Le Meilleur Quiz", MB_OK | MB_ICONERROR);
    return 1;
  }

  strncpy(fileUrl, htmlPath, sizeof(fileUrl) - 1);
  fileUrl[sizeof(fileUrl) - 1] = '\0';
  normalize_to_file_url_path(fileUrl);

  {
    char prefixedUrl[4096];
    _snprintf(prefixedUrl, sizeof(prefixedUrl) - 1, "file:///%s", fileUrl);
    prefixedUrl[sizeof(prefixedUrl) - 1] = '\0';
    strncpy(fileUrl, prefixedUrl, sizeof(fileUrl) - 1);
    fileUrl[sizeof(fileUrl) - 1] = '\0';
  }

  if (launch_browser_app_mode("msedge.exe", fileUrl)) {
    return 0;
  }
  if (launch_browser_app_mode("chrome.exe", fileUrl)) {
    return 0;
  }

  if ((INT_PTR)ShellExecuteA(NULL, "open", htmlPath, NULL, NULL, SW_SHOWNORMAL) <= 32) {
    MessageBoxA(NULL, "Impossible d'ouvrir l'application.", "Le Meilleur Quiz", MB_OK | MB_ICONERROR);
    return 1;
  }

  return 0;
}
