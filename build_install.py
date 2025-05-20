import os
import time
import jinja2
import shutil
import platform
import subprocess
from conan import conan_version
from contextlib import contextmanager


def recursive_glob(root_dir: str = '.', suffix: str = ''):
    return [os.path.join(looproot, filename)
            for looproot, _, filenames in os.walk(root_dir)
            for filename in filenames if filename.endswith(suffix)]


@contextmanager
def chdir(dir_path):
    current = os.getcwd()
    os.makedirs(dir_path, exist_ok=True)
    os.chdir(dir_path)
    try:
        yield
    finally:
        os.chdir(current)


def replace(file_path, text, replace):
    with open(file_path, "r") as f:
        content = f.read()
    content2 = content.replace(text, replace)
    assert content != content2
    with open(file_path, "w") as f:
        f.write(content2)


def load(file_path):
    with open(file_path, "r") as f:
        content = f.read()
    return content


@contextmanager
def tmp_dir(newdir):
    os.makedirs(newdir)
    try:
        with chdir(newdir):
            yield
    finally:
        shutil.rmtree(newdir)


def run(cmd, error=False):
    print("Running: {}".format(cmd))
    start_time = time.time()

    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True, text=True)

    output = ''

    for line in iter(process.stdout.readline, ''):
        print(line, end='', flush=True)
        output += line

    ret = process.wait()
    end_time = time.time()

    elapsed_time = end_time - start_time
    print(f"Elapsed time: {elapsed_time:.2f} seconds")

    if ret != 0 and not error:
        raise Exception(f"Failed cmd: {cmd}\n{output}")
    if ret == 0 and error:
        raise Exception(f"Cmd succeeded (failure expected): {cmd}\n{output}")

    return output


def replace(file_path, text, replace):
    with open(file_path, "r") as f:
        content = f.read()
    content2 = content.replace(text, replace)
    assert content != content2
    with open(file_path, "w") as f:
        f.write(content2)


def load(file_path):
    with open(file_path, "r") as f:
        content = f.read()
    return content

def setup_layout(root_path: str):
    clear_layout(root_path)
    os.makedirs("install", exist_ok=True)
    os.makedirs("build/Debug", exist_ok=True)
    os.makedirs("build/Release", exist_ok=True)

def clear_layout(root_path: str):
    if os.path.exists(os.path.join(root_path, "install")):
        shutil.rmtree(os.path.join(root_path, "install"))
    if os.path.exists(os.path.join(root_path, "build")):
        shutil.rmtree(os.path.join(root_path, "build"))

def build_install(build_type: str, root_path: str, qt_path: str, c_compiler: str, cxx_compiler: str):
    shutil.copytree(f"{root_path}/cmake/package_manager",
                    f"{root_path}/build/{build_type}/package_manager", dirs_exist_ok=True)
    auto_setup = os.path.join(root_path, "build", build_type, "package_manager", "auto-setup.cmake")
    with chdir(f"build/{build_type}"):
        if (platform.system() != "Windows"):
            cmd_out = run(f"cmake -DCMAKE_PROJECT_INCLUDE_BEFORE:FILEPATH=\"{auto_setup}\" "\
                      "-DBUILD_SHARED_LIBS:BOOL=ON "\
                      "-DCMAKE_GENERATOR:STRING=Ninja " \
                      "-DBuildInsParamTester:BOOL=OFF "\
                      f"-DCMAKE_BUILD_TYPE:STRING=\"{build_type}\" "\
                      f"-DCMAKE_PREFIX_PATH:PATH=\"{qt_path}\" " \
                      f"-DCMAKE_C_COMPILER:FILEPATH=\"{c_compiler}\" "\
                      f"-DCMAKE_CXX_COMPILER:FILEPATH=\"{cxx_compiler}\" "\
                      "-DCMAKE_INSTALL_PREFIX:PATH=../../install ../..")
            cmd_out = run(f"cmake --build . --config {build_type} --target install")
        else:
            cmd_out = run(f"cmake -DCMAKE_PROJECT_INCLUDE_BEFORE:FILEPATH=\"{auto_setup}\" "\
                      "-DBUILD_SHARED_LIBS:BOOL=ON "\
                      "-DCMAKE_GENERATOR:STRING=Ninja " \
                      "-DBuildInsParamTester:BOOL=OFF "\
                      "-DUseQtWidgets:BOOL=TRUE "\
                      f"-DCMAKE_BUILD_TYPE:STRING=\"{build_type}\" "\
                      f"-DCMAKE_PREFIX_PATH:PATH=\"{qt_path}\" " \
                      f"-DCMAKE_C_COMPILER:FILEPATH=\"{c_compiler}\" "\
                      f"-DCMAKE_CXX_COMPILER:FILEPATH=\"{cxx_compiler}\" "\
                      "-DCMAKE_INSTALL_PREFIX:PATH=../../install ../..")
            cmd_out = run(f"cmake --build . --config {build_type} --target install")
    return cmd_out

def lib_packager(user: str, build_type: str, root_path: str, lib_path: str):
    os_type: str = "Linux" if (platform.system() != "Windows") else "Windows"
    channel: str = build_type.lower()

    version_path = os.path.join(lib_path, "version.txt")
    with open(version_path, "r") as fp:
        pro_info = fp.read()
    pro_infos = str(pro_info).split(":")
    pro_infos = {"name":pro_infos[0].rstrip('\n'), "version":pro_infos[1].rstrip('\n')}

    requires_path = os.path.join(lib_path, "requires.txt")
    with open(requires_path, "r") as fp:
        requires = fp.read().splitlines()
    print(requires)

    template = jinja2.Template(conanfile_autogen())
    conan_file_path_dst = os.path.join(root_path, "install", pro_infos["name"], "conanfile.py")
    with open(conan_file_path_dst, "w") as fp:
        conan_file_dst = template.render(requires=requires, lib_name=pro_infos["name"], lib_version=pro_infos["version"])
        print(conan_file_dst)
        fp.write(conan_file_dst)

    lib_name = pro_infos["name"].lower();
    lib_version = pro_infos["version"];
    cmd_out = run(f"conan remove \"{lib_name}/{lib_version}@{user}/{channel}\" -v -c")
    with chdir("install/{}".format(pro_infos["name"])):
        cmd_out = run(f"conan export-pkg . -s os={os_type} -s arch=x86_64 -s build_type={build_type} --channel={channel}")
    return cmd_out, pro_infos

def lib_upload(user: str, build_type: str, lib_name: str, lib_version: str):
    lib_name: str = lib_name.lower()
    channel: str = build_type.lower()
    return run(f"conan upload \"{lib_name}/{lib_version}@{user}/{channel}\" --remote lksense -v --check --force")

def pipeline(root_path: str,
             src_dir: str,
             user: str,
             build_type: str,
             lib_name: str,
             lib_version: str,
             qt_path: str,
             c_compiler: str,
             cxx_compiler: str):
    cmd_out = build_install(user=user, build_type=build_type, root_path=root_path, qt_path=qt_path, c_compiler=c_compiler, cxx_compiler=cxx_compiler)
    cmd_out = lib_packager(build_type=build_type, src_dir=src_dir, lib_name=lib_name)
    cmd_out = lib_upload(user=user, build_type=build_type, lib_name=lib_name, lib_version=lib_version)
    return cmd_out

def conanfile_autogen():
    conan_tmpl_str = "from conan import ConanFile\n"\
                     "from conan.tools.files import copy, collect_libs\n"\
                     "from contextlib import contextmanager\n"\
                     "import os\n\n"\
                     "required_conan_version = \">=2.0.5\" \n\n\n"\
                     "@contextmanager\n"\
                     "def chdir(dir_path):\n"\
                     "    current = os.getcwd()\n"\
                     "    os.makedirs(dir_path, exist_ok=True)\n"\
                     "    os.chdir(dir_path)\n"\
                     "    try:\n"\
                     "        yield\n"\
                     "    finally:\n"\
                     "        os.chdir(current)\n\n\n"\
                     "class {{ lib_name }}Recipe(ConanFile):\n"\
                     "    revision_mode: str = \"scm\" \n"\
                     "    user: str = \"lksense2\" \n"\
                     "    name: str = \"{{ lib_name }}\".lower() \n"\
                     "    version: str = \"{{ lib_version }}\" \n"\
                     "    author = \"<JohnLiu> <liuhuijun@lksense.com>\"\n"\
                     "    url = \"<...>\" \n"\
                     "    description = \"{{ lib_name }}\" \n"\
                     "    topics = (\"LinkSenseAIFI\", \"BaseModules\", \"{{ lib_name }}\") \n"\
                     "    settings = \"os\", \"build_type\", \"arch\" \n\n"\
                     "    def requirements(self):\n"\
                     "        channel: str = str(self.settings.build_type).lower()\n"\
                     "        {% for require in requires %}self.requires(f\"{{require}}\") \n"\
                     "        {% endfor %} \n\n"\
                     "    def layout(self):\n"\
                     "        _bt = str(self.settings.build_type)\n"\
                     "        self.folders.build = _bt\n"\
                     "        self.folders.source = self.folders.build\n"\
                     "        self.cpp.source.includedirs = [\"include\"]\n"\
                     "        self.cpp.build.libdirs = [\"lib\"] \n\n"\
                     "    def package(self):\n"\
                     "        local_include_folder = os.path.join(self.source_folder, self.cpp.source.includedirs[0])\n"\
                     "        local_lib_folder = os.path.join(self.build_folder, self.cpp.build.libdirs[0])\n\n\n"\
                     "        copy(self, \"*.h\", local_include_folder, os.path.join(self.package_folder, \"include\"), keep_path=True)\n"\
                     "        copy(self, \"*\", local_lib_folder, os.path.join(self.package_folder, \"lib\"), keep_path=True)\n"\
                     "        if self.settings.os == \"Linux\":\n"\
                     "            with chdir(os.path.join(self.package_folder, \"lib\")):\n"\
                     "                lib_name = collect_libs(self)[0]\n"\
                     "                os.rename(\"{}.so\".format(lib_name), \"{}.so\".format(lib_name.lower()))\n\n"\
                     "    def package_info(self):\n"\
                     "        self.cpp_info.libs = collect_libs(self)\n"
    return conan_tmpl_str

if __name__ == '__main__':
    USER = "lksense2"

    MSVC: str = "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.41.34120/bin/HostX64/x64/cl.exe"
    QT_PATH: str = "/opt/Qt/6.2.4/gcc_64" if (platform.system() != "Windows") else "C:/Qt/6.2.4/msvc2019_64"
    C_COMPILER: str = "/usr/local/gcc-13.3.0/bin/gcc-13.3.0" if (platform.system() != "Windows") else MSVC
    CXX_COMPILER: str = "/usr/local/gcc-13.3.0/bin/g++-13.3.0" if (platform.system() != "Windows") else MSVC
    BUILD_TYPES: list = ["Release", "Debug"]

    root_path: str = os.getcwd()
    setup_layout(root_path=root_path)
    if (platform.system() != "Windows"):
        version_files = recursive_glob(root_dir=root_path + "/InsParam", suffix="version.txt")
    else:
        version_files = [root_path + "/InsParam/version.txt", root_path + "/InsParamUi/version.txt"]
    for version_path in version_files:
        lib_path = os.path.dirname(version_path)
        print(f"lib_path: {lib_path}")
        for build_type in BUILD_TYPES:
            cmd_out = build_install(build_type=build_type, root_path=root_path, qt_path=QT_PATH, c_compiler=C_COMPILER, cxx_compiler=CXX_COMPILER)
            cmd_out, pro_infos = lib_packager(user=USER, build_type=build_type, root_path=root_path, lib_path=lib_path)
            cmd_out = lib_upload(user=USER, build_type=build_type, lib_name=pro_infos["name"], lib_version=pro_infos["version"])

    clear_layout(root_path=root_path)
