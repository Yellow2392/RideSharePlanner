#!/bin/bash

set -e

# Colores para output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Funciones de utilidad
print_step() {
  echo -e "${GREEN}[INFO]${NC} $1"
}

print_error() {
  echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
  echo -e "${YELLOW}[WARN]${NC} $1"
}

# Función para compilar el proyecto
compile_project() {
  print_step "Compilando el proyecto..."

  # Crear directorio build si no existe
  if [ ! -d "build" ]; then
    mkdir build
    print_step "Directorio build creado"
  fi

  cd build

  # Configurar con cmake
  print_step "Configurando con CMake..."
  cmake .. -DCMAKE_BUILD_TYPE=Release

  # Compilar con todos los cores disponibles
  print_step "Compilando..."
  make -j$(nproc)

  cd ..

  # Verificar que los ejecutables se crearon
  if [ -f "build/bin/BenchmarkSuite" ]; then
    print_step "✓ BenchmarkSuite compilado exitosamente"
  else
    print_error "Falló la compilación de BenchmarkSuite"
    exit 1
  fi

  if [ -f "build/bin/RideSharePlanner" ]; then
    print_step "✓ RideSharePlanner compilado exitosamente"
  else
    print_error "Falló la compilación de RideSharePlanner"
    exit 1
  fi

  print_step "Compilación completada"
}

# Función para limpiar archivos de compilación
clean_project() {
  print_step "Limpiando archivos de compilación..."

  if [ -d "build" ]; then
    rm -rf build
    print_step "✓ Directorio build eliminado"
  else
    print_warning "No hay directorio build para limpiar"
  fi

  # Opcional: limpiar resultados de benchmark
  read -p "¿Eliminar también resultados de benchmark? (y/N): " -n 1 -r
  echo
  if [[ $REPLY =~ ^[Yy]$ ]]; then
    if [ -d "benchmark_results" ]; then
      rm -rf benchmark_results
      print_step "✓ Resultados de benchmark eliminados"
    fi
  fi

  print_step "Limpieza completada"
}

# Función para ejecutar benchmarks
run_benchmark() {
  local benchmark_type=$1

  # Verificar que el ejecutable existe
  if [ ! -f "build/bin/BenchmarkSuite" ]; then
    print_error "BenchmarkSuite no encontrado. Ejecuta primero: $0 compile"
    exit 1
  fi

  # Crear directorio de resultados
  mkdir -p benchmark_results

  print_step "Ejecutando benchmark: $benchmark_type"

  # Ejecutar el benchmark correspondiente
  case $benchmark_type in
  "quick")
    ./build/bin/BenchmarkSuite --quick
    ;;
  "full")
    ./build/bin/BenchmarkSuite --full
    ;;
  "requests")
    ./build/bin/BenchmarkSuite --requests
    ;;
  "vehicles")
    ./build/bin/BenchmarkSuite --vehicles
    ;;
  "capacity")
    ./build/bin/BenchmarkSuite --capacity
    ;;
  "deadline")
    ./build/bin/BenchmarkSuite --deadline
    ;;
  "all")
    ./build/bin/BenchmarkSuite --all
    ;;
  *)
    print_error "Tipo de benchmark desconocido: $benchmark_type"
    echo "Tipos válidos: quick, full, requests, vehicles, capacity, deadline, all"
    exit 1
    ;;
  esac

  print_step "Benchmark completado"
  print_step "Resultados guardados en: benchmark_results/"
}

# Función para ejecutar el programa original
run_original() {
  if [ ! -f "build/bin/RideSharePlanner" ]; then
    print_error "RideSharePlanner no encontrado. Ejecuta primero: $0 compile"
    exit 1
  fi

  print_step "Ejecutando programa original..."
  ./build/bin/RideSharePlanner
}

# Función para mostrar ayuda
show_help() {
  echo "Script de Benchmarking para Ride-Sharing"
  echo ""
  echo "Uso: $0 [comando]"
  echo ""
  echo "Comandos:"
  echo "  compile              - Compilar el proyecto"
  echo "  clean                - Limpiar archivos de compilación"
  echo "  test                 - Ejecutar programa original"
  echo "  benchmark [tipo]     - Ejecutar benchmark específico"
  echo ""
  echo "Tipos de benchmark:"
  echo "  quick                - Benchmark rápido (pruebas)"
  echo "  full                 - Benchmark completo (investigación)"
  echo "  requests             - Variar número de requests"
  echo "  vehicles             - Variar número de vehículos"
  echo "  capacity             - Variar capacidad de vehículos"
  echo "  deadline             - Variar deadlines"
  echo "  all                  - Todos los benchmarks"
  echo ""
  echo "Ejemplos:"
  echo "  $0 compile           # Compilar proyecto"
  echo "  $0 benchmark quick   # Benchmark rápido"
  echo "  $0 benchmark requests # Solo variar requests"
  echo "  $0 clean             # Limpiar todo"
  echo ""
  echo "Flujo típico:"
  echo "  1. $0 compile        # Compilar una vez"
  echo "  2. $0 benchmark quick # Ejecutar benchmarks"
  echo "  3. $0 clean          # Limpiar cuando termines"
}

if [ $# -eq 0 ]; then
  show_help
  exit 1
fi

if [ ! -f "CMakeLists.txt" ]; then
  print_error "Este script debe ejecutarse desde el directorio raíz del proyecto"
  print_error "Debe contener CMakeLists.txt"
  exit 1
fi

case "$1" in
"compile")
  compile_project
  ;;
"clean")
  clean_project
  ;;
"test")
  compile_project
  run_original
  ;;
"benchmark")
  if [ -z "$2" ]; then
    print_error "Especifica el tipo de benchmark"
    echo "Tipos disponibles: quick, full, requests, vehicles, capacity, deadline, all"
    exit 1
  fi
  compile_project
  run_benchmark "$2"
  ;;
"help" | "--help" | "-h")
  show_help
  ;;
*)
  print_error "Comando desconocido: $1"
  echo ""
  show_help
  exit 1
  ;;
esac

print_step "Operación completada exitosamente"
