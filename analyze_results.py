import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import os
import argparse
from pathlib import Path

plt.style.use('seaborn-v0_8-whitegrid')
sns.set_palette("husl")

class BenchmarkAnalyzer:
    def __init__(self, results_dir="benchmark_results"):
        self.results_dir = Path(results_dir)
        self.figures_dir = self.results_dir / "figures"
        self.figures_dir.mkdir(exist_ok=True)
        
        self.algorithm_colors = {
            'GAS': '#1f77b4',      # Azul
            'GAS-O1': '#ff7f0e',   # Naranja  
            'GAS-O2': '#2ca02c',   # Verde
        }
        
        # Marcadores
        self.algorithm_markers = {
            'GAS': 'o',
            'GAS-O1': 's', 
            'GAS-O2': '^'
        }
    
    def load_data(self, filename="benchmark_results.csv"):
        filepath = self.results_dir / filename
        if not filepath.exists():
            raise FileNotFoundError(f"No se encontró el archivo: {filepath}")
        
        df = pd.read_csv(filepath)
        print(f"Datos cargados: {len(df)} registros")
        print(f"Algoritmos: {df['algorithm'].unique()}")
        print(f"Tipos de parámetros: {df['parameter_type'].unique()}")
        
        return df
    
    def aggregate_results(self, df):
        aggregated = df.groupby(['algorithm', 'parameter_type', 'parameter_value']).agg({
            'total_revenue': ['mean', 'std'],
            'execution_time_ms': ['mean', 'std'],
            'memory_usage_mb': ['mean', 'std'],
            'service_rate': ['mean', 'std'],
            'revenue_per_request': ['mean', 'std']
        }).reset_index()
        
        aggregated.columns = ['_'.join(col).strip() if col[1] else col[0] 
                             for col in aggregated.columns.values]
        
        return aggregated
    
    def plot_revenue_vs_parameter(self, df, parameter_type, title_suffix=""):
        # Filtrar datos para el tipo de parámetro específico
        data = df[df['parameter_type'] == parameter_type].copy()
        
        if data.empty:
            print(f"No hay datos para parameter_type: {parameter_type}")
            return
        
        agg_data = self.aggregate_results(data)
        
        plt.figure(figsize=(10, 6))
        
        for algorithm in agg_data['algorithm'].unique():
            alg_data = agg_data[agg_data['algorithm'] == algorithm]
            
            alg_data = alg_data.sort_values('parameter_value')
            
            plt.plot(alg_data['parameter_value'], 
                    alg_data['total_revenue_mean'],
                    marker=self.algorithm_markers[algorithm],
                    color=self.algorithm_colors[algorithm],
                    label=algorithm,
                    linewidth=2,
                    markersize=8)
            
            if 'total_revenue_std' in alg_data.columns:
                plt.errorbar(alg_data['parameter_value'], 
                           alg_data['total_revenue_mean'],
                           yerr=alg_data['total_revenue_std'],
                           color=self.algorithm_colors[algorithm],
                           alpha=0.3,
                           capsize=3)
        
        plt.xlabel(self._get_parameter_label(parameter_type), fontsize=12)
        plt.ylabel('Total Revenue', fontsize=12)
        plt.title(f'Revenue vs {self._get_parameter_label(parameter_type)}{title_suffix}', 
                 fontsize=14, fontweight='bold')
        plt.legend(fontsize=11)
        plt.grid(True, alpha=0.3)
        
        filename = f'revenue_vs_{parameter_type}{title_suffix.lower().replace(" ", "_")}.png'
        plt.savefig(self.figures_dir / filename, dpi=300, bbox_inches='tight')
        plt.show()
        
        print(f"Gráfica guardada: {self.figures_dir / filename}")
    
    def plot_performance_comparison(self, df, parameter_type="requests"):
        data = df[df['parameter_type'] == parameter_type].copy()
        
        if data.empty:
            print(f"No hay datos para parameter_type: {parameter_type}")
            return
        
        agg_data = self.aggregate_results(data)
        
        fig, axes = plt.subplots(2, 2, figsize=(15, 12))
        fig.suptitle(f'Performance Analysis vs {self._get_parameter_label(parameter_type)}', 
                    fontsize=16, fontweight='bold')
        
        # 1. Revenue
        ax1 = axes[0, 0]
        for algorithm in agg_data['algorithm'].unique():
            alg_data = agg_data[agg_data['algorithm'] == algorithm].sort_values('parameter_value')
            ax1.plot(alg_data['parameter_value'], alg_data['total_revenue_mean'],
                    marker=self.algorithm_markers[algorithm],
                    color=self.algorithm_colors[algorithm],
                    label=algorithm, linewidth=2, markersize=6)
        
        ax1.set_xlabel(self._get_parameter_label(parameter_type))
        ax1.set_ylabel('Total Revenue')
        ax1.set_title('(a) Total Revenue')
        ax1.legend()
        ax1.grid(True, alpha=0.3)
        
        # 2. Execution Time
        ax2 = axes[0, 1]
        for algorithm in agg_data['algorithm'].unique():
            alg_data = agg_data[agg_data['algorithm'] == algorithm].sort_values('parameter_value')
            ax2.semilogy(alg_data['parameter_value'], alg_data['execution_time_ms_mean'],
                        marker=self.algorithm_markers[algorithm],
                        color=self.algorithm_colors[algorithm],
                        label=algorithm, linewidth=2, markersize=6)
        
        ax2.set_xlabel(self._get_parameter_label(parameter_type))
        ax2.set_ylabel('Execution Time (ms)')
        ax2.set_title('(b) Execution Time')
        ax2.legend()
        ax2.grid(True, alpha=0.3)
        
        # 3. Memory Usage
        ax3 = axes[1, 0]
        for algorithm in agg_data['algorithm'].unique():
            alg_data = agg_data[agg_data['algorithm'] == algorithm].sort_values('parameter_value')
            ax3.plot(alg_data['parameter_value'], alg_data['memory_usage_mb_mean'],
                    marker=self.algorithm_markers[algorithm],
                    color=self.algorithm_colors[algorithm],
                    label=algorithm, linewidth=2, markersize=6)
        
        ax3.set_xlabel(self._get_parameter_label(parameter_type))
        ax3.set_ylabel('Memory Usage (MB)')
        ax3.set_title('(c) Memory Usage')
        ax3.legend()
        ax3.grid(True, alpha=0.3)
        
        # 4. Service Rate
        ax4 = axes[1, 1]
        for algorithm in agg_data['algorithm'].unique():
            alg_data = agg_data[agg_data['algorithm'] == algorithm].sort_values('parameter_value')
            ax4.plot(alg_data['parameter_value'], alg_data['service_rate_mean'],
                    marker=self.algorithm_markers[algorithm],
                    color=self.algorithm_colors[algorithm],
                    label=algorithm, linewidth=2, markersize=6)
        
        ax4.set_xlabel(self._get_parameter_label(parameter_type))
        ax4.set_ylabel('Service Rate')
        ax4.set_title('(d) Request Service Rate')
        ax4.legend()
        ax4.grid(True, alpha=0.3)
        
        plt.tight_layout()
        
        filename = f'performance_comparison_{parameter_type}.png'
        plt.savefig(self.figures_dir / filename, dpi=300, bbox_inches='tight')
        plt.show()
        
        print(f"Gráfica guardada: {self.figures_dir / filename}")
    
    def generate_summary_table(self, df):
        summary_stats = []
        
        for param_type in df['parameter_type'].unique():
            param_data = df[df['parameter_type'] == param_type]
            
            for algorithm in param_data['algorithm'].unique():
                alg_data = param_data[param_data['algorithm'] == algorithm]
                
                stats = {
                    'Parameter Type': param_type,
                    'Algorithm': algorithm,
                    'Avg Revenue': f"{alg_data['total_revenue'].mean():.2f}",
                    'Std Revenue': f"{alg_data['total_revenue'].std():.2f}",
                    'Avg Time (ms)': f"{alg_data['execution_time_ms'].mean():.2f}",
                    'Avg Service Rate': f"{alg_data['service_rate'].mean():.3f}",
                    'Max Revenue': f"{alg_data['total_revenue'].max():.2f}",
                    'Min Revenue': f"{alg_data['total_revenue'].min():.2f}"
                }
                summary_stats.append(stats)
        
        summary_df = pd.DataFrame(summary_stats)
        
        summary_file = self.results_dir / "summary_statistics.csv"
        summary_df.to_csv(summary_file, index=False)
        
        print("\n" + "="*80)
        print("RESUMEN ESTADÍSTICO")
        print("="*80)
        print(summary_df.to_string(index=False))
        print(f"\nTabla guardada en: {summary_file}")
        
        return summary_df
    
    def plot_algorithm_comparison_heatmap(self, df):
        comparison_data = []
        
        for param_type in df['parameter_type'].unique():
            param_data = df[df['parameter_type'] == param_type]
            
            for param_value in param_data['parameter_value'].unique():
                subset = param_data[param_data['parameter_value'] == param_value]
                
                revenues = {}
                for alg in subset['algorithm'].unique():
                    alg_revenues = subset[subset['algorithm'] == alg]['total_revenue']
                    revenues[alg] = alg_revenues.mean()
                
                if 'GAS-O2' in revenues and 'GAS' in revenues and 'GAS-O1' in revenues:
                    improvement_vs_gas = ((revenues['GAS-O2'] - revenues['GAS']) / revenues['GAS']) * 100
                    improvement_vs_o1 = ((revenues['GAS-O2'] - revenues['GAS-O1']) / revenues['GAS-O1']) * 100
                    
                    comparison_data.append({
                        'Parameter Type': param_type,
                        'Parameter Value': param_value,
                        'GAS-O2 vs GAS (%)': improvement_vs_gas,
                        'GAS-O2 vs GAS-O1 (%)': improvement_vs_o1
                    })
        
        if comparison_data:
            comp_df = pd.DataFrame(comparison_data)
            
            plt.figure(figsize=(12, 8))
            
            pivot_data = comp_df.pivot_table(
                values='GAS-O2 vs GAS (%)', 
                index='Parameter Type', 
                columns='Parameter Value', 
                aggfunc='mean'
            )
            
            sns.heatmap(pivot_data, annot=True, cmap='RdYlGn', center=0, 
                       fmt='.1f', cbar_kws={'label': 'Revenue Improvement (%)'})
            
            plt.title('GAS-O2 Revenue Improvement vs GAS (%)', fontsize=14, fontweight='bold')
            plt.xlabel('Parameter Value')
            plt.ylabel('Parameter Type')
            plt.tight_layout()
            
            filename = 'algorithm_comparison_heatmap.png'
            plt.savefig(self.figures_dir / filename, dpi=300, bbox_inches='tight')
            plt.show()
            
            print(f"Heatmap guardado: {self.figures_dir / filename}")
    
    def _get_parameter_label(self, parameter_type):
        labels = {
            'requests': 'Number of Requests',
            'vehicles': 'Number of Vehicles', 
            'capacity': 'Vehicle Capacity',
            'deadline': 'Deadline (seconds)'
        }
        return labels.get(parameter_type, parameter_type.capitalize())
    
    def run_complete_analysis(self, filename="benchmark_results.csv"):
        print("="*60)
        print("ANÁLISIS COMPLETO DE RESULTADOS DE BENCHMARK")
        print("="*60)
        
        df = self.load_data(filename)
        
        for param_type in df['parameter_type'].unique():
            print(f"\nGenerando gráficas para: {param_type}")
            self.plot_revenue_vs_parameter(df, param_type)
            self.plot_performance_comparison(df, param_type)
        
        self.generate_summary_table(df)
        
        self.plot_algorithm_comparison_heatmap(df)
        
        print(f"\n{'='*60}")
        print("ANÁLISIS COMPLETADO")
        print(f"Todas las visualizaciones guardadas en: {self.figures_dir}")
        print(f"{'='*60}")

def main():
    parser = argparse.ArgumentParser(description='Analizar resultados de benchmark de ride-sharing')
    parser.add_argument('--file', '-f', default='benchmark_results.csv',
                       help='Archivo CSV con resultados (default: benchmark_results.csv)')
    parser.add_argument('--output-dir', '-o', default='benchmark_results',
                       help='Directorio de resultados (default: benchmark_results)')
    parser.add_argument('--parameter', '-p', choices=['requests', 'vehicles', 'capacity', 'deadline'],
                       help='Analizar solo un tipo de parámetro específico')
    
    args = parser.parse_args()
    
    analyzer = BenchmarkAnalyzer(args.output_dir)
    
    try:
        if args.parameter:
            # Análisis específico de un parámetro
            df = analyzer.load_data(args.file)
            analyzer.plot_revenue_vs_parameter(df, args.parameter)
            analyzer.plot_performance_comparison(df, args.parameter)
        else:
            # Análisis completo
            analyzer.run_complete_analysis(args.file)
            
    except FileNotFoundError as e:
        print(f"Error: {e}")
        
    except Exception as e:
        print(f"Error durante el análisis: {e}")

if __name__ == "__main__":
    main()
