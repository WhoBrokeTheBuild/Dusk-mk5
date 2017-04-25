#!/bin/sh

function get_distro()
{
  which pacman  >/dev/null && { echo arch; return; }
  which yum &   >/dev/null && { echo redhat; return; }
  which apt-get >/dev/null && { echo debian; return; }
}

distro=`get_distro`

case $distro in
  arch)
    sudo pacman -S \
      make \
      cmake \
      gcc \
      glew \
      freeglut \
      zlib \
      soil
    ;;
  redhat)
    sudo yum install -y \
      make \
      cmake \
      gcc \
      glew-devel \
      freeglut-devel \
      SOIL-devel
    ;;
debian)
	sudo apt-get install -y \
		make \
		cmake \
		gcc \
    libglew-dev \
    freeglut-dev \
    zlib1g-dev \
    libsoil-dev
	;;
  * )
    echo "Unsupported Distro"
  ;;
esac

git clone git@github.com:dvidelabs/flatcc.git /tmp/flatcc
cd /tmp/flatcc
scripts/initbuild.sh make-concurrent
scripts/build.sh
